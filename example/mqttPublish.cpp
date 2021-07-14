
#include <mosquitto.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sstream>


void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
    printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
    if(reason_code != 0){
        mosquitto_disconnect(mosq);
    }

}

void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
    printf("Message with mid %d has been published.\n", mid);
}


std::string getMessage()
{
        std::ifstream file("/Users/bair/Desktop/SummerPractic2021/example/data.txt"); 
        std::string message;

        if (file.is_open()){    
            std::string line;

            while (getline(file, line)){
                std::istringstream iss(line); 
                std::string token;
                int i = 0;

                while (getline(iss, token, '\n')) {
                    if (i == 0) message = token; 
                    i++;
                }
            }
        }
        file.close();
        return message;
}

void publishData(struct mosquitto *mosq)
{
    char payload[20];
    std::string temp;
    int rc;
    temp = getMessage();
    rc = mosquitto_publish(mosq, NULL, "position", temp.size(), temp.c_str(), 2, false);
    if(rc != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
    }
}


int main(int argc, char *argv[])
{
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
    while (1){
        publishData(mosq);
        sleep(1);
    }
    

    mosquitto_lib_cleanup();
    return 0;
}
