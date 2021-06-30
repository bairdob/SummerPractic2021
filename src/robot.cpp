//robot.hpp
#include <iostream>

class Point{
protected:
	int x;
	int y;
public:
	Point(){
		x = 0;
		y = 0; 
	};

	int get_x(){return x;};

	int get_y(){return y;};

	Point get_point(){
		Point p;
		p.x = get_x();
		p.y = get_y();
		return p;
	}
	
	//~Point();
	//int z; //optional
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

class Robot:public Point{
private:
	Point position;
	int azimuth; //angle 
	//Camera resolution;

public:
	Robot(){// toDO constucotor with params
		Point();
		azimuth = 0;
	};

	int get_azimuth(Point position, Point object){
		//toDO
		return azimuth;
	};

	float get_distance(Point position, Point object){
		float distance;
		//toDO
		return distance;
	};

	void rotate(int azimuth){ //rotate robot or without params
		//toDO if without add get_azimuth()
	};


	void move(int azimuth, float distance){//move robot or without params
		//toDo if without add get_azimuth(), get_distance()
	};

	void show(){
		std::cout << "position = ("<< position.get_x() << "," << position.get_y() << ") angle = " << azimuth << std::endl;
	}

	// Robot get_robot_data(){ toDO Fix
	// 	Robot r;
	// 	r.position = get_point();
	// 	r.azimuth = azimuth;
	// 	return r;
	// }
	//~Robot();
	
};

int main(){
	Robot r1;
	r1.show();
	return 0;
}