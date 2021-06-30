//robot.hpp
#include <iostream>

class Point{
protected:
	float x;
	float y;
public:
	Point(){
		x = 0;
		y = 0; 
	};
	float get_x(){return x;};
	float get_y(){return y;};
	//~Point();
	//float z; //optional
};

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
	Robot(){// toDO constucotor with params
		Point();
		azimuth = 0;
	};

	float get_azimuth(Point position, Point object){
		//toDO
		return azimuth;
	};

	float get_distance(Point position, Point object){
		float distance;
		//toDO
		return distance;
	};

	void rotate(float azimuth){ //rotate robot or without params
		//toDO if without add get_azimuth()
	};


	void move(float azimuth, float distance){//move robot or without params
		//toDo if without add get_azimuth(), get_distance()
	};

	void show(){
		std::cout << "position = ("<< position.get_x() << "," << position.get_y() << ") angle = " << azimuth << std::endl;
	}
	//~Robot();
	
};

int main(){
	Robot r1;
	r1.show();
	return 0;
}