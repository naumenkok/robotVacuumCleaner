#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <array>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <stdexcept>

struct Point {
    size_t x, y;
    inline bool operator!=(const Point& p) {
        return (x != p.x || y != p.y);
    }
    inline bool operator==(const Point& p) {
        return (x == p.x && y == p.y);
    }
};

constexpr size_t COLUMNS = 25, ROWS = 14;
using my_array = std::array<std::array<int, COLUMNS>, ROWS>;
static std::vector<std::pair<Point, Point>> Obstacles;


class Map
{
private:
    int rows;
    int columns;
    my_array map2d;
public:
    Map();
    Map(my_array& map2d);
    int getRows();
    int getColumns();
    my_array getMap();
    void setMap(my_array& map2d);
    void setValue(int x, int y, int value);
    void addWalls();
    void addObstacle(const size_t x1, const size_t y1, const size_t x2, const size_t y2);
    bool checkIfAllCleaned();
    bool allSidesUnavailable(const size_t x1, const size_t y1);
    void printMap();
    bool operator==(my_array& m2);
};