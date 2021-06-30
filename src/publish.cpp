#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void on_connect(struct mosquitto *mosq, void *obj, int reason_code){
	printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
	if(reason_code != 0){
		mosquitto_disconnect(mosq);
	}

}

void on_publish(struct mosquitto *mosq, void *obj, int mid){
	printf("Message with mid %d has been published.\n", mid);
}

int get_temperature(void){
	sleep(1);
	return random()%100;
}

void publish_robot_data(struct mosquitto *mosq){
	char payload[20];
	int x, y, azimuth;
	int rc;

	x = get_x();
	y = get_y();
	azimuth = get_azimuth();

	snprintf(payload, sizeof(payload), "%d", x);

	rc = mosquitto_publish(mosq, NULL, "robot/x", strlen(payload), payload, 2, false);
	if(rc != MOSQ_ERR_SUCCESS){
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
	}

	snprintf(payload, sizeof(payload), "%d", y);
	rc = mosquitto_publish(mosq, NULL, "robot/y", strlen(payload), payload, 2, false);
	if(rc != MOSQ_ERR_SUCCESS){
		fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
	}

	snprintf(payload, sizeof(payload), "%d", azimuth);
	rc = mosquitto_publish(mosq, NULL, "robot/angle", strlen(payload), payload, 2, false);
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

	rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 60);
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


	while(1){
		publish_robot_data(mosq);
	}

	mosquitto_lib_cleanup();
	return 0;
}