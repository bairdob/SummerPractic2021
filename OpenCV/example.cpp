#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// Function Headers
void detectAndDisplay(Mat frame);

// Global variables
string face_cascade_name = "haarcascade_frontalface_alt2.xml";
CascadeClassifier face_cascade;

// Function main
int main(void)
{
    // Load the cascade
    if (!face_cascade.load(face_cascade_name)){
        printf("--(!)Error on cascade loading\n");
        return (-1);
    }

    // Read the image file
    Mat frame = imread("lena.jpg");

    // Apply the classifier to the frame
    if (!frame.empty())
        detectAndDisplay(frame);
    waitKey(0);
    return 0;
}

// Function detectAndDisplay
void detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;
    
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    // Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    for (int ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)
    {
        Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
        Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
    }

    imshow("original", frame);
}