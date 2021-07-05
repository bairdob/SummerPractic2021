#include <opencv2/opencv.hpp>
using namespace cv;
int main()
{
 Mat img(Mat::zeros(300, 300, CV_8U));
 imshow("window", img);
 waitKey();
 return 0;
}