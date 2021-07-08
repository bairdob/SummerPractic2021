// WebcamTest.cpp

#include <opencv2/opencv.hpp>

#include <iostream>

int main()
{
  // declare a VideoCapture object and associate to webcam, 1 => use 2nd webcam, the 0th webcam is the one integral to the TX2 development board
  cv::VideoCapture capWebcam(1);

  // check if VideoCapture object was associated to webcam successfully, if not, show error message and bail
  if (capWebcam.isOpened() == false)
  {
    std::cout << "error: capWebcam not accessed successfully\n\n";
    return (0);
  }

  cv::Mat imgOriginal;        // input image
  cv::Mat imgGrayscale;       // grayscale of input image
  cv::Mat imgBlurred;         // intermediate blured image
  cv::Mat imgCanny;           // Canny edge image

  char charCheckForEscKey = 0;

  // while the Esc key has not been pressed and the webcam connection is not lost . . .
  while (charCheckForEscKey != 27 && capWebcam.isOpened())
  {
    bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);            // get next frame

    // if frame was not read successfully, print error message and jump out of while loop
    if (!blnFrameReadSuccessfully || imgOriginal.empty())
    {
      std::cout << "error: frame not read from webcam\n";
      break;
    }

    // convert to grayscale
    cv::cvtColor(imgOriginal, imgGrayscale, cv::COLOR_BGR2GRAY);

    // blur image
    cv::GaussianBlur(imgGrayscale, imgBlurred, cv::Size(5, 5), 0);

    // get Canny edges
    cv::Canny(imgBlurred, imgCanny, 75, 150);

    cv::imshow("imgOriginal", imgOriginal);
    cv::imshow("imgCanny", imgCanny);

    charCheckForEscKey = cv::waitKey(1);        // delay (in ms) and get key press, if any
  }  // end while

  return (0);
}