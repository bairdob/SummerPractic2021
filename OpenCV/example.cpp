#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    //initialize and allocate memory to load the video stream from camera 
    cv::VideoCapture camera0(2);
    cv::VideoCapture camera1(1);

    if( !camera0.isOpened() ) return 1;
    if( !camera1.isOpened() ) return 1;

    while(true) {
        //grab and retrieve each frames of the video sequentially 
        cv::Mat3b frame0;
        camera0 >> frame0;
        cv::Mat3b frame1;
        camera1 >> frame1;

        cv::imshow("Video0", frame0);
        cv::imshow("Video1", frame1);

        //wait for 40 milliseconds
        int c = waitKey(40);

        //exit the loop if user press "Esc" key  (ASCII value of "Esc" is 27) 
        if(27 == char(c)) break;
    }

    // Get the frame
Mat save_img; camera0 >> save_img;
if(save_img.empty())
{
  std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
}
// Save the frame into a file
imwrite("test1.jpg", save_img); // A JPG FILE IS BEING SAVED

    return 0;
}