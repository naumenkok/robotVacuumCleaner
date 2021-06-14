#include"DrawnMap.h"

int main()
{
	std::array<std::array<int, COLUMNS>, ROWS> map2d = { 0 };
	size_t started_x = 1, started_y = 1;
	int x = started_x, y = started_y;



	Map m1(map2d);
	m1.addWalls();
	m1.addObstacle(2, 3, 6, 4);
	m1.addObstacle(8, 7, 10, 10);
	DrawnMap dm(&m1, "Room");
	dm.drawAll(x, y);
}