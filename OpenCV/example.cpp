#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace std;
void roi(Mat& img,int x_point,int y_point,int width ,int height);

int main(int argc, char** argv) {

    string image_path; // image path for input image

    if(argc < 2)
        image_path = "test.jpg";
    else 
        image_path = argv[1];

    // declare images 
    Mat src, gray, blur_image, threshold_output;

    // take input image
    src = imread(image_path, 1);
  
    // convert to grayscale 
    cvtColor(src, gray, COLOR_BGR2GRAY);
  
    // add blurring to the input image
    blur(gray, blur_image, Size(10, 10));
  
    // binary threshold the input image
    threshold(gray, threshold_output, 200, 255, THRESH_BINARY);
  
    // show source image
    namedWindow("Source", WINDOW_AUTOSIZE);
    imshow("Source", src);
  
    // contours vector  
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
  
    // find contours for the thresholded image
    findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
  
    // create empty black image
    Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);

    // BoundyBox vector  
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Rect> objects;

    for( size_t i = 0; i < contours.size(); i++ ){ 
        if( contours[i].size() > 500 & contours[i].size() < 2000){ //grab big objects
            approxPolyDP( contours[i], contours_poly[i], 3, true );
            boundRect[i] = boundingRect( contours_poly[i] );
            objects.push_back(boundRect[i]);
        }
    }

    /// Get the moments
    vector<Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){ 
        mu[i] = moments( contours[i] );
    }

    ///  Get the mass centers
    vector<Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){
        if(contours[i].size() > 500 & contours[i].size() < 2000){ //grab only detecting objects
        //add 1e-5 to avoid division by zero
        mc[i] = Point2f(static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)), static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
        cout << "mc[" << i << "]=" << mc[i] << endl;
        }
    }
    
    // // draw contours on the empty black image 
    // for(int i = 0; i < contours.size(); i++) {
    //     Scalar color_contours = Scalar(0, 255, 0); 
    //     Scalar color_box = Scalar(0, 0, 255);
    //     Scalar color_center = Scalar(0, 255, 255);
    //     // draw contours
    //     drawContours(drawing, contours, i, color_contours, 2, 8, vector<Vec4i>(), 0, Point());
    //     rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color_box, 2 );
    //     drawMarker( drawing, mc[i], color_center, 0, 50 , 10);
    // }


    //demonstration start

    // Mat roiImg = src(Rect(boundRect[0].height, boundRect[0].width, boundRect[0].x, boundRect[0].y)); 

    for(int i = 0; i < contours.size(); i++) {
        Scalar color_contours = Scalar(0, 255, 0); 
        Scalar color_box = Scalar(0, 0, 255);
        Scalar color_center = Scalar(0, 255, 255);
        // draw contours
        drawContours(src, contours, i, color_contours, 2, 8, vector<Vec4i>(), 0, Point());
        rectangle( src, boundRect[i].tl(), boundRect[i].br(), color_box, 2 );

    }
    
    //roi(src,boundRect[0].height, boundRect[0].width, boundRect[0].x, boundRect[0].y);
    namedWindow("Compare contouring", WINDOW_AUTOSIZE);
    imshow("Compare contouring", src);
    waitKey();
    destroyAllWindows();

    //equalizeHist(roiImg,roiImg);              // perform equalization of ROI
    for (auto it : objects) {
        Mat roi = Mat(src,it);
        namedWindow("Compare contouring", WINDOW_AUTOSIZE);
        imshow("Compare contouring", roi);
        waitKey();
        destroyAllWindows();
    }
    
    return 0;

}
