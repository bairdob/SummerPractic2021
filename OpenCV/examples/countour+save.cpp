// CannyTutorial.cpp : Defines the entry point for the console application. 
// Environment: Visual studio 2015, Windows 10
// Assumptions: Opecv is installed configured in the visual studio project
// Opencv version: OpenCV 3.1

//#include "stdafx.h"
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<string>
#include<iostream>



int main()
{

    //Modified from source: https://github.com/MicrocontrollersAndMore/OpenCV_3_Windows_10_Installation_Tutorial
    cv::Mat imgOriginal;        // input image
    cv::Mat imgGrayscale;        // grayscale of input image
    cv::Mat imgBlurred;            // intermediate blured image
    cv::Mat imgCanny;            // Canny edge image

    std::cout << "Please enter an image filename : ";
    std::string img_addr = "test.jpg";
    //std::cin >> img_addr;

    //std::cout << "Searching for " + img_addr << std::endl;

    imgOriginal = cv::imread(img_addr);            // open image

    if (imgOriginal.empty()) {                                    // if unable to open image
        std::cout << "error: image not read from file\n\n";        // show error message on command line
        return(0);                                                // and exit program
    }

    cv::cvtColor(imgOriginal, imgGrayscale,  cv::COLOR_BGRA2GRAY);        // convert to grayscale

    cv::GaussianBlur(imgGrayscale,            // input image
        imgBlurred,                            // output image
        cv::Size(5, 5),                        // smoothing window width and height in pixels
        100);                                // sigma value, determines how much the image will be blurred

    cv::Canny(imgBlurred,            // input image
        imgCanny,                    // output image
        50,                        // low threshold
        60);                        // high threshold


    // Declare windows
    // Note: you can use CV_WINDOW_NORMAL which allows resizing the window
    // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
    // CV_WINDOW_AUTOSIZE is the default
    cv::namedWindow("imgOriginal", cv::WINDOW_AUTOSIZE);        
    cv::namedWindow("imgCanny", cv::WINDOW_AUTOSIZE);

    //Show windows
    cv::imshow("imgOriginal", imgOriginal);        
    cv::imshow("imgCanny", imgCanny);
    imwrite("test_filtered.jpg", imgCanny);
    cv::waitKey(0);                    // hold windows open until user presses a key
    return 0;
}