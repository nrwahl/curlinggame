//Nathan Wahl
//Program 2
//Program.h
//3/23/2014
//This header contains class declarations for the three main classes of the curling program
//Implementation is contained in NRW_Program2.cpp

#pragma once
#include <vector>

class Point 
{
	//Acts as a standardized point class for use in polygons etc. Basic methods
	//Default point is 0,0
	
private:
	float x, y;
public:
	Point(float x, float y); //Create point at x,y
	Point(void);
	void setPoint(float x, float y) { this->x = x; this->y = y; } //Change value of point
	float getX() { return x; }
	float getY() { return y; }
};



class Rect
{	
	//Rect Class
	//This class acts as the base class for most of the program. In the future I would like to seperate out the stone specifics
	//as an inherited class, but currently it stands as is. Most functions are self explanatory

public:
	Rect(Point center, float width, float height); //Creates default white filled rect at center point
	Rect(Point centerPoint, float rectWidth, float rectHeight, bool powerBar); //Constructor for powerbar sets appropriate values
	void move(void); //Moves rectangle based on velocity
	void show(void); //Function to display rectangle, called by displayFunc();
	void setColor(float R, float G, float B); //Sets rectangle fill color
	void scale(float dX, float dY); //For this program multiplicative scaling not needed
									//Adds dX, dY to width and height of object
	void rotate(int rotationDegree); //Rotate by degrees
	void fireStone(void);	//Used to fire the rectangle as a stone based on starting location and angle
	bool checkCollision(Rect rectangleB); //Checks if colliding with another Rect

	void setCenter(Point newCenter) { center = newCenter; } //Moves Rect to a specific point
	void setXVelocity(float velocity) { xVelocity = velocity; } //Set x component of velocity
	void setYVelocity(float velocity) { yVelocity = velocity; } //Set y component of velocity
	void setPower(int powerVar) { power = powerVar; } //Sets total power that a stone can move. -1 is unlimited.
	void setPowerBar(bool ispowerBar) { powerBar = ispowerBar; } //Flags rectangle as a powerbar, unique properties
	void setRotation(int degrees); //Sets the rotation to a specific one, overriding what was already there
	void setIsFired(bool fired) { isFired = fired; } //Flags this rect as being fired as a stone
	void setScore(int points) { score = points; } //Sets the point value of this rect as a stone 
	
	Point getCenter(void) { return center; }
	float getXVelocity(void) { return xVelocity; }
	float getYVelocity(void) { return yVelocity; }
	double getHeight(void) { return height; }
	double getWidth(void) { return width; }
	int getPower(void) { return power; }
	bool getIsFired(void) { return isFired; }
	bool isPowerBar(void) { return powerBar; }
	int getScore(void) { return score; }

private:
	float power; //For stones, determines how much motion is still available for the stone, set to negative for infinite motion
	bool powerBar;
	void createVertices(); //Only called by member functions. 
	void createVerticesPB(); //Creates vertices based on bottom and height. Specific numbers.
	Point center;	//Center point
	float red;		//Amount red
	float green;	//Amount green
	float blue;		//Amoun blue
	double height;
	double width;
	float rotation; //Total rotation from default position in degrees
	Point point[4]; //Points based on center, length, width, and rotation
	float xVelocity, yVelocity; //Velocity values for motion
	bool isFired;
	int score; //Point value of stone
};

class Player
{

	//This class contains all the details on creating and moving the player. Player is a circle and thus requires 
	//A more complicated show() function, implementing the Bresenham Circle Algorithm.
	//Only contains y velocity as is only capable in moving up and down
public:
	
	Player(Point center); //Creates Player at a point, radius is hardcoded
	void move(void); //Moves player based on applied velocity
	void show(void); //Draws player, called by displayFunc();
	void rotate(int rotationDegree); //Apply additional rotation to player 
	
	void setYVelocity(float velocity) { yVelocity = velocity; }
	void setRotation(int degrees) { rotation = degrees; } //Same as rotation for Rect
	void setCenter(Point newCenter) { center = newCenter; }

	float getYVelocity(void) { return yVelocity; }
	float getRotation(void) { return rotation; }
	int getRadius(void) { return radius; }
	Point getCenter(void) { return center; }

private:
	Point center;
	int radius;
	int rotation; //Total rotation from default position in degrees
	float yVelocity;
	Point frontLinePoints[2]; //Points for the targeting line depicting the front
	void createVertices();	  //Creates vertices for show algorithm
	std::vector<Point> circlePoints; //Array of points depicting the approximate circle
	void drawCircle(Point center, int X, int Y); //Portion of Bresenham circle drawing algorithm
};