//mqtt_publish.cpp
/*

*/
#include <vector>
#include <iostream>
//#include <cstdlib>
#include <sstream> 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <fstream>
#include <sstream>


const int MIN_SIZE_CONTOUR = 500;
const int MAX_SIZE_CONTOUR = 2000;

std::vector<cv::Point2f> getCoord(std::vector< std::vector<cv::Point> > contours){
	// Get the moments
    std::vector<cv::Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){ 
        mu[i] = moments( contours[i] );
    }
    std::vector<cv::Point2f> output;
    //  Get the mass centers
    std::vector<cv::Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){
        if(contours[i].size() > MIN_SIZE_CONTOUR & contours[i].size() < MAX_SIZE_CONTOUR){ //grab only detecting objects
        	//add 1e-5 to avoid division by zero
        	mc[i] = cv::Point2f(static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)), static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
        	std::cout << "mc[" << i << "]=" << mc[i] << std::endl;
        	output.push_back(mc[i]);
        }
    }
    return output;
}


std::string getStringFromVPosition(std::vector<cv::Point2f> position){//toDO
	std::stringstream buffer;
	for (auto it : position) buffer << it << " ";
	// std::cout << buffer.str();
	std::string str = buffer.str();
	return str;
}

void writeToFile(std::string str){
    std::ofstream out("/Users/bair/Desktop/SummerPractic2021/example/data.txt", std::ofstream::app);

    if (out.is_open()){
        out  << str << std::endl;
    }

    out.close();    
}

int main(){
	std::vector<cv::Point2f> test = {{1,2}, {3,4}};
	// for (auto it:test) std::cout << it << " ";
	// getStringFromVPosition(test);
    writeToFile(getStringFromVPosition(test));
	return 0;
}