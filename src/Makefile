all: publish 

publish: publish.o 
	g++ publish.o -o publish -lmosquitto

publish.o: publish.cpp
	g++ -c publish.cpp -I /usr/local/Cellar/mosquitto/2.0.9/include

subscribe: subscribe.o 
	g++ subscribe.o -o subscribe -lmosquitto

subscribe.o: subscribe.cpp
	g++ -c subscribe.cpp -I /usr/local/Cellar/mosquitto/2.0.9/include
	
clean:
	rm -rf *.o publish subscribe