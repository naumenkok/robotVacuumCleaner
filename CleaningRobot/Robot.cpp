#include "Robot.h"

Robot::Robot(int x, int y) {
	this->x0 = x;
	this->y0 = y;
}

int Robot::getX()
{
	return x0;
}

int Robot::getY()
{
	return y0;
}

void Robot::setX(int x) {
	this->x0 = x;
}

void Robot::setY(int y) {
	this->y0 = y;
}

void Robot::setPosition(int x, int y) {
	this->x0 = x;
	this->y0 = y;
}
