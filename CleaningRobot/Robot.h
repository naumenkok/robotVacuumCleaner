#pragma once
#include<iostream>
#include "Map.h"
using namespace std;

class Robot
{
private:
    int x0;
    int y0;
public:
    Robot(int x, int y);
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    void setPosition(int x, int y);
};
