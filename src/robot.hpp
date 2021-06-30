//robot.hpp
#include <iostream>

class Point{
protected:
	float x;
	float y;
	//float z; //optional
}

// class Camera{ //maybe need for future features
// protected:
// 	int mp; // maybe enum 720p, 960p,1080p, 2160p etc

// public:
// 	Point get_resolution (int mp) {
// 		//code
// 		return Point;	// return height, width or not Point if enum
// 	}; 
// }

class Robot{
private:
	Point position;
	float azimuth; //angle 
	//Camera resolution;

public:
	Robot();

	azimuth get_azimuth(Point position, Point object){
		//toDO
		return azimuth;
	};

	float get_distance(Point position, Point object){
		//toDO
		return distance;
	};

	void rotate(float azimuth){ //rotate robot or without params
		//toDO if without add get_azimuth()
	};


	void move(float azimuth, float distance){//move robot or without params
		//toDo if without add get_azimuth(), get_distance()
	};

	~Robot();
	
};