#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

void processFrame(Mat& img, Mat& frame);//Drawing an external rectangle

int main(int argc, char** argv){
    VideoCapture capture;
    capture.open("IMG_5178.MOV");

    if (!capture.isOpened()){
         cout << "Video file not found..." << endl;
        return -1;
    }

    // declare images 
    Mat frame;
    Mat gray, blur_image, threshold_output;

    while (capture.read(frame))
    {
         // convert to grayscale 
        cvtColor(frame, gray, COLOR_BGR2GRAY);
      
        // add blurring to the input image
        blur(gray, blur_image, Size(15, 15));
      
        // binary threshold the input image
        threshold(blur_image, threshold_output, 160, 255, THRESH_BINARY);

        // imshow("output video", threshold_output);
        processFrame(threshold_output, frame);
        
        imshow("input video", frame);

        char c = waitKey(50);
        if (c == 27){
            break;
        }

    }

    capture.release();
    waitKey(0);

    return 0;
}

void processFrame(Mat & img, Mat &frame){
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
            cout << "mc[" << i << "]=" << mc[i] << endl;
            
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
    for (auto it : objects) {
        Mat roi = Mat(frame,it);
        objects_mat.push_back(roi);
        // namedWindow("Compare contouring", WINDOW_AUTOSIZE);
        // imshow("Compare contouring", roi);
        // waitKey();
        // destroyAllWindows();
    }

    // vector<Mat> objects_mat_hsv(objects_mat.size());
    // for (int i = 0; i < objects_mat.size(); i++){
    //     cvtColor(objects_mat[i], objects_mat_hsv[i], COLOR_BGR2HSV );
    // }


    // int h_bins = 50; int s_bins = 60;
    // int histSize[] = { h_bins, s_bins };

    // float h_ranges[] = { 0, 180 };
    // float s_ranges[] = { 0, 256 };

    // const float* ranges[] = { h_ranges, s_ranges };

    // int channels[] = { 0, 1 };

    // std::vector<Mat> hist(objects_mat.size());
    // for (int i = 0; i < objects_mat.size(); i++){
    //     calcHist( &objects_mat_hsv[i], 1, channels, Mat(), hist[i], 2, histSize, ranges, true, false );
    //     normalize( hist[i], hist[i], 0, 1, NORM_MINMAX, -1, Mat() );
    // }
    // cout << objects_mat.size();
}
