all:  mqttPublish mqttSubscribe

mqttSubscribe: mqttSubscribe.o 
	g++ mqttSubscribe.o -o mqttSubscribe -lmosquitto

mqttSubscribe.o: mqttSubscribe.cpp
	g++ -c mqttSubscribe.cpp -I /usr/local/Cellar/mosquitto/2.0.9/include

mqttPublish: mqttPublish.o 
	g++ mqttPublish.o -o mqttPublish -lmosquitto

mqttPublish.o: mqttPublish.cpp
	g++ -c mqttPublish.cpp -I /usr/local/Cellar/mosquitto/2.0.9/include
	
clean:
	rm -rf *.o  mqttPublish mqttSubscribe