//Nathan Wahl
//Program 2
//-----------------------------------------------
//NRW_Program2.cpp
//3/23/2014
//This is the main implementation file for the curling game. The declarations for the three classes used here
//Are located in Program2.h
//This game is a basic curling game
//Press <spacebar> once to start the power meter. Press again to fire, throwing a stone across the court
//Press <u> or <U> to move the player up, <j> or <J> to move down.
//Press <i> or <I> to rotate CW or <k> or <K> to rotate CCW
//Player is limited to 5 stones
//Stones rotate as they move across the court
//Power is randomized by the condition of the ice

//*** SPECIAL FEATURES ***\\

//Stones will collide with eachother, allowing the player to bump their stones in and out
//Game can be reset upon completion by pressing <r> or <R>

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
#include <GL/glut.h>
#include "Program2.h"
#include "cs_graphics_setup.h"
#include <iostream>
#include <random>

using namespace std;

//Constants
#define WINDOW_XS 900
#define WINDOW_YS 500
#define WINDOW_NAME "NRW Curling Game"
#define ANI_MSEC 10
#define POWER_BAR_SPEED 5
#define PLAYER_MOVE_SPEED 4
#define PLAYER_ROTATION_SPEED 3
#define STONE_MOVE_SPEED 5
const double PI = 3.141592653589793238463;

//Function Prototypes
void display_func(void);
void keyboard_func(unsigned char c, int x, int y);
void animation_func(int val);
void reset_game();
void display_text();


//Object declarations (global)
bool powerBarUp = true;
bool powerBarMove = false; 
int stoneCount = 0;
int spaceCount = 0;
float stonePower = 0;
float condition = 0;
bool canFire = true;
int playerScore = 0;
//Strings
string lastName = "Wahl";
string gameName = "Curling Game";
string gameOver = "Game Over!";
string playAgain = "Press 'r' to play again, or 'q' to quit.";

//Points
Point goalCenter(60, 200);
Point powerBar(870, 200);
Point playerStart(800, 200);
Point stoneStart(760, 200);

//Rectangles
Rect greenRect(goalCenter, 100, 100);
Rect yellowRect(goalCenter, 70, 70);
Rect redRect(goalCenter, 40, 40);
Rect staticPowerBar(powerBar, 30, 300);
Rect dynamicPowerBar(powerBar, 30, 0, true);

Player player(playerStart);
vector<Rect> stone; //Vector containing all the stones

Rect stone1(stoneStart, 20, 20);
Rect stone2(stoneStart, 20, 20);
Rect stone3(stoneStart, 20, 20);
Rect stone4(stoneStart, 20, 20);
Rect stone5(stoneStart, 20, 20);
Rect stone6(stoneStart, 20, 20);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	init_setup(WINDOW_XS, WINDOW_YS, WINDOW_NAME);

	//Initialize proper values for rectangles
	greenRect.setColor(0, 1, 0);
	yellowRect.setColor(1, 1, 0);
	redRect.setColor(1, 0, 0);
	//Setup game
	reset_game();

	//Enter main loop
	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutTimerFunc(ANI_MSEC, animation_func, 0);
	glutMainLoop();

	return 1;
}

//Display Function
void display_func(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	//Black line for divider
	glColor3f(0, 0, 0); 
	glBegin(GL_LINE_LOOP);
	glVertex2i(0, 400);
	glVertex2i(900, 400);
	glEnd();
	
	//Show objects
	greenRect.show();
	yellowRect.show();
	redRect.show();
	staticPowerBar.show();
	dynamicPowerBar.show();
	player.show();
	
	//Show active stones on screen
	for (int i = 0; i < stone.size(); i++)
	{
		if (i <= stoneCount && i <= 4)
		{
			stone[i].show();
		}
	}

	//Display text at top
	display_text();

	glFlush();
	glutSwapBuffers();
}

//Animation Function
void animation_func(int val)
{

	//Logic for power bar
	if (powerBarMove)
	{
		if (powerBarUp)
		{
			dynamicPowerBar.scale(0, POWER_BAR_SPEED);
		}
		else
		{
			dynamicPowerBar.scale(0, -1 * POWER_BAR_SPEED);
		}
		if (dynamicPowerBar.getHeight() > 300)
		{
			dynamicPowerBar.scale(0, -1 * POWER_BAR_SPEED);
			powerBarUp = false;
		}
		if (dynamicPowerBar.getHeight() < 0)
		{
			dynamicPowerBar.scale(0, POWER_BAR_SPEED);
			powerBarUp = true;
		}
	}

	//Move stones that should move
	for (int i = 0; i < stoneCount; i++)
	{
		stone[i].move();
	}

	//Check collision for all stones on field
	for (int i = stoneCount; i > 0; i--)
	{
		if (stone[i].checkCollision(stone[i - 1]) && stone[i].getIsFired())
		{
			stone[i - 1].setPower(stone[i].getPower());
			stone[i - 1].setXVelocity(stone[i].getXVelocity());
			stone[i - 1].setYVelocity(stone[i].getYVelocity());
			stone[i].setPower(0);
		}
	}

	//Check collision for points
	for (int i = 0; i < stoneCount; i++)
	{
		if (stone[i].getCenter().getX() > 40 && stone[i].getCenter().getX() < 80 && 
			stone[i].getCenter().getY() > 180 && stone[i].getCenter().getY() < 220)
		{
			stone[i].setScore(5);
		}
		else if (stone[i].getCenter().getX() > 25 && stone[i].getCenter().getX() < 95 &&
			stone[i].getCenter().getY() > 165 && stone[i].getCenter().getY() < 235)
		{
			stone[i].setScore(3);
		}
		else if (stone[i].getCenter().getX() > 10 && stone[i].getCenter().getX() < 110 &&
			stone[i].getCenter().getY() > 100 && stone[i].getCenter().getY() < 300)
		{
			stone[i].setScore(1);
		}
		else
		{
			stone[i].setScore(0);
		}
	}
	
	//Calculate player score
	playerScore = 0;
	for (int i = 0; i < stoneCount; i++)
	{
		playerScore += stone[i].getScore();
	}

	//Determines if player can fire again or not
	canFire = true;
	for (int i = 0; i < stoneCount; i++)
	{
		if (stone[i].getPower() != 0)
		{
			canFire = false;
		}
	}
	if (stoneCount >= 5)
	{
		canFire = false;
	}

	glutPostRedisplay();
	glutTimerFunc(ANI_MSEC, animation_func, 0);
}

//Function to reset game to starting settings
void reset_game(void)
{
	//Reset counters
	stoneCount = 0;
	spaceCount = 0;
	//Clear stone buffer
	stone.clear();
	//Replace stones onto buffer
	stone.push_back(stone1);
	stone.push_back(stone2);
	stone.push_back(stone3);
	stone.push_back(stone4);
	stone.push_back(stone5);
	stone.push_back(stone6);
	//Reset player position
	player.setCenter(playerStart);
	player.move();
	player.setRotation(0);
	playerScore = 0;
	//Reset stone statistics
	for (int i = 0; i < 5; i++)
	{
		stone[i].setCenter(stoneStart);
		stone[i].setIsFired(false);
		stone[i].setPower(-1);
		stone[i].setXVelocity(0);
		stone[i].setYVelocity(0);
		stone[i].setScore(0);
		stone[i].setRotation(0);
	}
}

//Keyboard Event function
void keyboard_func(unsigned char c, int x, int y)
{
	//Move player up and down
	if (c == 'u' || c == 'U')
	{
		player.setYVelocity(PLAYER_MOVE_SPEED);
		player.move();
		//Move stones that have not been played with player
		for (int i = 0; i < stone.size(); i++)
		{
			bool fired = stone[i].getIsFired(); 
			if (fired == 0)
			{
				stone[i].setYVelocity(PLAYER_MOVE_SPEED);
				stone[i].move();
			}
		}

		if (player.getCenter().getY() > 360)
		{
			player.setYVelocity(-PLAYER_MOVE_SPEED);
			player.move();
			for (int i = 0; i < stone.size(); i++)
			{
				bool fired = stone[i].getIsFired();
				if (fired == 0)
				{
					stone[i].setYVelocity(-PLAYER_MOVE_SPEED);
					stone[i].move();
				}

			}
		}
		player.setYVelocity(0);
		stone[stoneCount].setYVelocity(0);
		glutPostRedisplay();
	}
	else if (c == 'j' || c == 'J')
	{
		player.setYVelocity(-PLAYER_MOVE_SPEED);
		player.move();
		for (int i = 0; i < stone.size(); i++)
		{
			bool fired = stone[i].getIsFired();
			if (fired == 0)
			{
				stone[i].setYVelocity(-PLAYER_MOVE_SPEED);
				stone[i].move();
			}
		}

		if (player.getCenter().getY() < 40)
		{
			player.setYVelocity(PLAYER_MOVE_SPEED);
			player.move();
			for (int i = 0; i < stone.size(); i++)
			{
				bool fired = stone[i].getIsFired();
				if (fired == 0)
				{
					stone[i].setYVelocity(PLAYER_MOVE_SPEED);
					stone[i].move();
				}
			}
		}
		player.setYVelocity(0);
		stone[stoneCount].setYVelocity(0);
		glutPostRedisplay();
	}
	//Logic for rotation
	else if (c == 'i' || c == 'I')
	{
		player.rotate(-PLAYER_ROTATION_SPEED);
		//Rotate stones that have not been played with player
		for (int i = 0; i < stone.size(); i++)
		{
			bool fired = stone[i].getIsFired();
			if (fired == 0)
			{
				stone[i].rotate(-PLAYER_ROTATION_SPEED);
			}
		}
		if (player.getRotation() < -45)
		{
			player.rotate(PLAYER_ROTATION_SPEED);
			for (int i = 0; i < stone.size(); i++)
			{
				bool fired = stone[i].getIsFired();
				if (fired == 0)
				{
					stone[i].rotate(PLAYER_ROTATION_SPEED);
				}
			}
		}
	}
	else if (c == 'k' || c == 'K')
	{
		player.rotate(PLAYER_ROTATION_SPEED);
		for (int i = 0; i < stone.size(); i++)
		{
			bool fired = stone[i].getIsFired();
			if (fired == 0)
			{
				stone[i].rotate(PLAYER_ROTATION_SPEED);
			}
		}
		if (player.getRotation() > 45)
		{
			player.rotate(-PLAYER_ROTATION_SPEED);
			for (int i = 0; i < stone.size(); i++)
			{
				bool fired = stone[i].getIsFired();
				if (fired == 0)
				{
					stone[i].rotate(-PLAYER_ROTATION_SPEED);
				}
			}
		}
	}
	//Logic for Spacebar
	else if (c == ' ' && spaceCount == 0 && canFire == true)
	{
		//Reset powerbar to start
		dynamicPowerBar.scale(0, -dynamicPowerBar.getHeight());
		//Move powerbar
		powerBarMove = true;
		//Increment space counter
		spaceCount++;
	}
	else if (c == ' ' && spaceCount == 1)
	{
		//Stop power bar
		powerBarMove = false;
		//Seed the random number generator
		srand(time(NULL));
		//Set the condition
		condition = ((rand() % 10) + 95);
		condition = condition * 0.01;
		//Calculate stone power (powerbar max height is 300 so * 3.33333 etc. gets close to 1000.
		//Supposed to be randomized anyways
	    stonePower = (dynamicPowerBar.getHeight() * 3.33334) * condition;
		//Fire the stone
		stone[stoneCount].fireStone();
		//Increment the stone counter
		stoneCount++;
		//Decrement spacecounter
		spaceCount--;
		//Stop user from firing till after animation
		canFire = false;
	}
	//Allow game to reset after it has been played
	else if ((c == 'r' || c == 'R') && stoneCount >= 5)
	{
		reset_game();
	}
	//Allow user to exit
	else if (c == 'q' || c == 'Q')
	{
		exit(0);
	}
}

//Function to display text on the screen
void display_text()
{
	string round = "Round: ";
	string score = "Score: ";
	int roundCount = stoneCount + 1;
	if (roundCount > 5)
	{
		roundCount = 5;
	}
	round.append(to_string(roundCount));
	score.append(to_string(playerScore));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0, 0, 0);
	glRasterPos2i(30, 470);
	void * font = GLUT_BITMAP_HELVETICA_18;
	for (string::iterator i = gameName.begin(); i != gameName.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}

	glRasterPos2i(30, 440);
	for (string::iterator i = lastName.begin(); i != lastName.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}

	glRasterPos2i(700, 470);
	for (string::iterator i = round.begin(); i != round.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}

	glRasterPos2i(700, 440);
	for (string::iterator i = score.begin(); i != score.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
	if (stoneCount > 4 && stone[4].getPower() == 0 && stone[3].getPower() == 0 && stone[2].getPower() == 0
		&& stone[1].getPower() == 0 && stone[0].getPower() == 0)
	{
		glRasterPos2i(350, 455);
		for (string::iterator i = gameOver.begin(); i != gameOver.end(); ++i)
		{
			char c = *i;
			glutBitmapCharacter(font, c);
		}
		glRasterPos2i(300, 430);
		for (string::iterator i = playAgain.begin(); i != playAgain.end(); ++i)
		{
			char c = *i;
			glutBitmapCharacter(font, c);
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//Point Class Implementation
//-------------------------------------------------------------------
Point::Point(float x, float y)
{
	setPoint(x, y);
}

Point::Point()
{
	setPoint(0, 0);
}

//Rect Class Implementation
//-------------------------------------------------------------------
Rect::Rect(Point centerPoint, float rectWidth, float rectHeight)
{
	//Initialize variables
	setCenter(centerPoint);
	height = rectHeight;
	width = rectWidth;
	setRotation(0);
	setXVelocity(0);
	setYVelocity(0);
	setColor(1, 1, 1);
	power = -1;
	isFired = false;
	score = 0;
	//Create vertices
	createVertices();
}

//Special constructor to denote altered behavior as powerbar
Rect::Rect(Point centerPoint, float rectWidth, float rectHeight, bool powerBar)
{
	//Initialize variables
	setCenter(centerPoint);
	height = rectHeight;
	width = rectWidth;
	setRotation(0);
	setXVelocity(0);
	setYVelocity(0);
	setColor(0, 0, 0.3);
	power = -1;
	setPowerBar(true);
	//Create vertices
	createVerticesPB();
}

void Rect::move(void)
{
	//If rectangle is allowed to move (not a stopped stone)
	if (power > 0 || power < 0)
	{
		center.setPoint((center.getX() + xVelocity), (center.getY() + yVelocity));
		//If this is a stone with power, adjust accordingly
		if (power > 0)
		{
			power -= STONE_MOVE_SPEED;
			//Rotate while moving stone
			rotate(5);
			//If power adjustment reduces it below 0, set to 0
			if (power <= 0)
			{
				power = 0;
			}
		}
		//Recreate vertices
		if (isPowerBar())
		{
			createVerticesPB();
		}
		else
		{
			createVertices();
		}
	}
}

void Rect::createVertices(void)
{
	//Calculate offsets
	float xOffset = 0.5*width;
	float yOffset = 0.5*height;
	//Set points
	point[0].setPoint((center.getX() + xOffset), (center.getY() - yOffset));
	point[1].setPoint((center.getX() + xOffset), (center.getY() + yOffset));
	point[2].setPoint((center.getX() - xOffset), (center.getY() + yOffset));
	point[3].setPoint((center.getX() - xOffset), (center.getY() - yOffset));
}

void Rect::createVerticesPB(void)
{
	float xOffset = 0.5*width;
	//Only X coordinate is dependent on center
	point[0].setPoint((center.getX() + xOffset), 50);
	point[1].setPoint((center.getX() + xOffset), 50 + height);
	point[2].setPoint((center.getX() - xOffset), 50 + height);
	point[3].setPoint((center.getX() - xOffset), 50);
}

void Rect::show(void)
{
	//Set color of rectangle
	glColor3f(red, green, blue); 
	//Create transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//Translate back to center (these are executed in reverse)
	glTranslatef(center.getX(), center.getY(), 0);
	//Rotate 
	glRotatef(rotation, 0, 0, 1);
	//Translate to origin 
	glTranslatef(-center.getX(), -center.getY(), 0);
	//Draw the rectangle
	glBegin(GL_POLYGON);
	for (int i = 0; i < 4; ++i)
	{
		glVertex2i(point[i].getX(), point[i].getY());
	}
	glEnd();
	//Create black outline
	glColor3f(0, 0, 0); 
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 4; ++i)
	{
		glVertex2i(point[i].getX(), point[i].getY());
	}
	glEnd();
	//Pop the matrix
	glPopMatrix();
}

void Rect::setColor(float R, float G, float B)
{
	red = R;
	green = G;
	blue = B;
}

void Rect::scale(float dX, float dY)
{
	width += dX;
	height += dY;
	
	if (isPowerBar())
	{
		createVerticesPB();
	}
	else
	{
		createVertices();
	}
}

void Rect::rotate(int degrees)
{
	rotation += degrees;
	while (rotation >= 360)
	{
		if (rotation >= 360)
		{
			rotation -= 360;
		}
	}
}

void Rect::setRotation(int degrees)
{
	rotation = degrees;
	while (rotation >= 360)
	{
		if (rotation >= 360)
		{
			rotation -= 360;
		}
	}
}

bool Rect::checkCollision(Rect rectangleB)
{
	//Create side of rectangles. If either side is inside the other rectangle, collision has occured
	float leftA, leftB;
	float rightA, rightB;
	float topA, topB;
	float bottomA, bottomB;
	Point rectBCenter = rectangleB.getCenter();

	//Calculate sides for A (calling rect)
	leftA = center.getX();
	rightA = center.getX() + width;
	topA = center.getY();
	bottomA = center.getY() + height;

	//Calculate sides for B
	leftB = rectBCenter.getX();
	rightB = rectBCenter.getX() + rectangleB.getWidth();
	topB = rectBCenter.getY();
	bottomB = rectBCenter.getY() + rectangleB.getHeight();

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}
	if (topA >= bottomB)
	{
		return false;
	}
	if (rightA <= leftB)
	{
		return false;
	}
	if (leftA >= rightB)
	{
		return false;
	}

	//If no sides of A are outside B
	return true;
}

void Rect::fireStone(void)
{
	//Set power to the determined value
	power = stonePower;
	//Apparently we need to do this calculation in radians
	double rotation = player.getRotation();
	rotation = rotation * (PI / 180); //Convert to radians
	//SOH CAH TOA!!!
	float dX = cos(rotation) * STONE_MOVE_SPEED;
	float dY = sin(rotation) * STONE_MOVE_SPEED;
	//Flip over axes for proper direction
	setXVelocity(-dX);
	setYVelocity(-dY);
	//Flag as fired
	isFired = true;
}

//Player Class Implementation
//-------------------------------------------------------------------
Player::Player(Point centerPoint)
{
	center = centerPoint;
	radius = 20;
	rotation = 0;
	yVelocity = 0; 
	createVertices(); //For the aiming line
}

void Player::move(void)
{
	center.setPoint(center.getX(), center.getY() + yVelocity);
	createVertices();
}

void Player::createVertices(void)
{
	//Create vertices for front line
	frontLinePoints[0].setPoint((center.getX() - 7), (center.getY() - 7));
	frontLinePoints[1].setPoint((center.getX() - 7), (center.getY() + 7));

	//Clear current coordinates
	circlePoints.clear();

	//Create vertices from bresenham algorithm
	int X = 0, Y = radius;
	int D = 3 - 2 * radius;
	while (X < Y)
	{
		drawCircle(center, X, Y);
		X++;
		if (D < 0)
		{
			D = D + (4 * X) + 6;
		}
		else
		{
			Y--;
			D = D + 4 * (X - Y) + 10;
		}
		drawCircle(center, X, Y);
	}
}

void Player::show(void)
{
	//Black circle
	glColor3f(0, 0, 0); 
	//Create transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//Rotate the inner line about the center
	glTranslatef(center.getX(), center.getY(), 0);
	glRotatef(rotation, 0, 0, 1);
	glTranslatef(-center.getX(), -center.getY(), 0);
	glBegin(GL_LINE_LOOP);
	//Draw inner line
	for (int i = 0; i < 2; i++)
	{
		glVertex2f(frontLinePoints[i].getX(), frontLinePoints[i].getY());
	}
	glEnd();
	//Rotating the circle looks bad. Its a circle why rotate it?
	glPopMatrix();
	//Draw circle from points
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < circlePoints.size(); i++)
	{
		glVertex2i(circlePoints[i].getX(), circlePoints[i].getY());
	}
	glEnd();
}

void Player::rotate(int degree)
{
	rotation += degree;
}

void Player::drawCircle(Point center, int X, int Y)
{
	//Create 8 points based on Bresenham algorithm
	//1
	Point temp(center.getX() + X, center.getY() + Y);
	circlePoints.push_back(temp);
	//2
	temp.setPoint(center.getX() - X, center.getY() + Y);
	circlePoints.push_back(temp);
	//3
	temp.setPoint(center.getX() + X, center.getY() - Y);
	circlePoints.push_back(temp);
	//4
	temp.setPoint(center.getX() - X, center.getY() - Y);
	circlePoints.push_back(temp);
	//5
	temp.setPoint(center.getX() + Y, center.getY() + X);
	circlePoints.push_back(temp);
	//6
	temp.setPoint(center.getX() - Y, center.getY() + X);
	circlePoints.push_back(temp);
	//7
	temp.setPoint(center.getX() + Y, center.getY() - X);
	circlePoints.push_back(temp);
	//8
	temp.setPoint(center.getX() - Y, center.getY() - X);
	circlePoints.push_back(temp);
}



