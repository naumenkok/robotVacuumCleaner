#pragma once
#include "Map.h"
using namespace std;

enum class Direction { Left, Right, Up, Down, Unknown };

class Track
{
private:
    vector<Point> points;
    Map* map;
public:
    Track(const size_t x0, const size_t y0, Map* map);
    vector<Point> getPoints();
    void addPoint(Point p);
    void deleteLastPoint();
    bool operator==(vector<Point>& tr2);
    vector<Point> getRoad(const size_t x0, const size_t y0);
    int checkRightSide(const size_t x, const size_t y, const Direction dir);
    int checkLeftSide(const size_t x, const size_t y, const Direction dir);
    Direction newDirection(const size_t x, const size_t y);
    Direction newDirectionExeption(const size_t x, const size_t y);
    Direction newDirectionExeptionLeft(const size_t x, const size_t y);
    vector<Point> moveBetweenPoints(const int x1, const int y1, const int x2, const int y2);
    Point findNearestZero(const size_t x1, const size_t y1);
    pair<Direction, Direction> newDirectionTwoTimes(const size_t x1, const size_t y1);
    pair<Direction, Direction> newDirectionTwoTimesExeption(const size_t x1, const size_t y1);
    pair<Direction, Direction> newDirectionTwoTimesExeptionLeft(const size_t x1, const size_t y1);
    vector<Point> avoidObstacleFromRight(const size_t x1, const size_t y1, vector<Point> orginalRoad);
    vector<Point> avoidObstacleFromLeft(const size_t x1, const size_t y1, vector<Point> orginalRoad);
    vector<Point> avoidObstacle(const size_t x1, const size_t y1, vector<Point> orginalRoad);


    Point step(size_t x0, size_t y0, Direction dir) noexcept;
    double lineInX(const int x, const pair<double, double> line);
    pair<double, double> line(const int x1, const int y1, const int x2, const int y2);
    bool lineGoesThroughtArea(const int x, const int y, const pair<double, double> line);
    double distance(Point p1, Point p2);
};

bool operator==(vector<Point>& tr1, vector<Point>& tr2);
bool operator==(Point& p1, Point& p2);
bool operator!=(Point& p1, Point& p2);