//mqtt_publish.cpp
/*

*/
#include <vector>
#include <iostream>
//#include <cstdlib>
#include <sstream> 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include <mosquitto.h>
#include <string>


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

void on_connect(struct mosquitto *mosq, void *obj, int reason_code){
    printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
    if(reason_code != 0){
        mosquitto_disconnect(mosq);
    }
}

void on_publish(struct mosquitto *mosq, void *obj, int mid){
    printf("Message with mid %d has been published.\n", mid);
}

void publish_data(struct mosquitto *mosq, std::vector<cv::Point2f> position){
    // char payload[20];
    std::string temp;
    int rc;
    temp = getStringFromVPosition(position);
    rc = mosquitto_publish(mosq, NULL, "position", temp.size(), temp.c_str(), 2, false);
    if(rc != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
    }
}



int main(){
	std::vector<cv::Point2f> test = {{1,2}, {3,4}};
	// for (auto it:test) std::cout << it << " ";
	// getStringFromVPosition(test);

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

    publish_data(mosq,test);
	return 0;
}