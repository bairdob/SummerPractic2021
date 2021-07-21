#include<opencv2/opencv.hpp>
#include<iostream>
#include <mosquitto.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

void processFrame(Mat& img, Mat& frame, string &msg);//Drawing an external rectangle


void on_connect(struct mosquitto *mosq, void *obj, int reason_code){
    printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
    if(reason_code != 0){
        mosquitto_disconnect(mosq);
    }

}

void on_publish(struct mosquitto *mosq, void *obj, int mid){
    printf("Message with mid %d has been published.\n", mid);
}


void publishData(struct mosquitto *mosq, string &str){
    char payload[20];
    std::string temp = str;
    int rc;
    // temp = getMessage();
    rc = mosquitto_publish(mosq, NULL, "position", temp.size(), temp.c_str(), 2, false);
    if(rc != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
    }
}


int main(int argc, char** argv){
    struct mosquitto *mosq;
    int rc;

    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if(mosq == NULL){
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_publish_callback_set(mosq, on_publish);

    rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        return 1;
    }

    rc = mosquitto_loop_start(mosq);
    if(rc != MOSQ_ERR_SUCCESS){
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        return 1;
    }

    VideoCapture capture;
    capture.open("IMG_5178.MOV");

    if (!capture.isOpened()){
         cout << "Video file not found..." << endl;
        return -1;
    }

    // declare images 
    Mat frame;
    Mat gray, blur_image, threshold_output;
    string msg;

    while (capture.read(frame)){
         // convert to grayscale 
        cvtColor(frame, gray, COLOR_BGR2GRAY);
      
        // add blurring to the input image
        blur(gray, blur_image, Size(15, 15));
        
        // binary threshold the input image
        threshold(blur_image, threshold_output, 160, 255, THRESH_BINARY);

        // imshow("output video", threshold_output);
        processFrame(threshold_output, frame, msg);
        publishData(mosq,msg);
        imshow("input video", frame);

        char c = waitKey(50);
        if (c == 27){
            break;
        }

    }

    capture.release();
    waitKey(0);

    mosquitto_lib_cleanup();
    return 0;
}

void processFrame(Mat & img, Mat &frame, string &msg){
    // contours vector  
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
  
    // find contours for the image
    findContours(img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_TC89_KCOS  , Point(0, 0));
    
    // BoundyBox vector  
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );

    // Object vector
    vector<Rect> objects;

    for( size_t i = 0; i < contours.size(); i++ ){ 
        if( contours[i].size() > 100 & contours[i].size() < 200){ //grab big objects
            approxPolyDP( contours[i], contours_poly[i], 3, true );
            boundRect[i] = boundingRect( contours_poly[i] );
            objects.push_back(boundRect[i]);
        }
    }

    // Get the moments
    vector<Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){ 
        mu[i] = moments( contours[i] );
    }

    //  Get the mass centers
    vector<Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){
        if(contours[i].size() > 100 & contours[i].size() < 200){ //grab only detecting objects
            //add 1e-5 to avoid division by zero
            mc[i] = Point2f(static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)), static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
            //cout << "mc[" << i << "]=" << mc[i] << endl;
        }
    }

    for(int i = 0; i < contours.size(); i++) {
        Scalar color_contours = Scalar(0, 255, 0); 
        Scalar color_box = Scalar(0, 0, 255);
        Scalar color_center = Scalar(0, 200, 200);
        // draw contours, bounding box, marker
        drawContours(frame, contours, i, color_contours, 2, 8, vector<Vec4i>(), 0, Point());
        rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color_box, 2 );
        drawMarker( frame, mc[i], color_center, 0, 30 , 5);
        // draw crosshair coordinates
        string disp = std::to_string(static_cast<int>(mc[i].x) ) + ", " + std::to_string(static_cast<int>(mc[i].y) ) ;
        putText(frame, disp, Point(mc[i].x, mc[i].y), FONT_HERSHEY_SIMPLEX, 1, color_center, 3);
    }
    
    vector<Rect> objects_hsv(objects.size());
    vector<Mat> objects_mat;
    for (auto &it : objects) {
        Mat roi = Mat(frame,it);
        objects_mat.push_back(roi);
        // namedWindow("Compare contouring", WINDOW_AUTOSIZE);
        // imshow("Compare contouring", roi);
        // waitKey();
        // destroyAllWindows();
    }    
    

    //worked text
    for( size_t i = 1; i < contours.size(); i++ ){ 
        string name;
        if (boundRect[i].width*boundRect[i].y > 80000){
            name = "pods";
            putText(frame, name, boundRect[i].tl(), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 3);
        } else  {
            name = "ball";
            putText(frame, name, boundRect[i].tl(), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 3);
        }
        if (mc[i].x != 0 ){
            string str = name + " = " + to_string(static_cast<int>(mc[i].x) ) + ","+ to_string(static_cast<int>(mc[i].y) );
            cout << str << endl;
            msg = str;
        }
    }

}

    


    


