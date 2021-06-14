#include "Track.h"

Track::Track(const size_t x0, const size_t y0, Map* map)
{
	this->map = map;
	points = getRoad(x0, y0);
}

vector<Point> Track::getPoints()
{
	return points;
}

void Track::addPoint(Point p)
{
	points.push_back(p);
}

void Track::deleteLastPoint()
{
	points.pop_back();
}

bool Track::operator==(vector<Point>& tr2)
{
	if (tr2.size() != this->points.size()) return false;
	for (int i = 0; i < this->points.size(); ++i)
		if (tr2[i].x != this->points[i].x || tr2[i].y != this->points[i].y)
			return false;
	return true;
}

vector<Point> Track::getRoad(const size_t x0, const size_t y0) {
	Point pt = { x0, y0 };
	std::vector<Point> steps = { pt };
	map->setValue(pt.x, pt.y, 2);
	Direction dir = Direction::Down;
	Point nextPt = step(pt.x, pt.y, dir);
	int nextPtContent = map->getMap()[nextPt.y][nextPt.x];
	int nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
	while (!map->checkIfAllCleaned())
	{
		if (nextPtRightSide && nextPtContent == 0)
		{
			pt = nextPt;
			map->setValue(pt.x, pt.y, 2);
			steps.push_back(pt);
			nextPt = step(pt.x, pt.y, dir);
			nextPtContent = map->getMap()[nextPt.y][nextPt.x];
			nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
		}
		else
		{
			Direction newDir = newDirection(pt.x, pt.y);
			if (newDir != Direction::Unknown)
			{
				pt = step(pt.x, pt.y, newDir);
				map->setValue(pt.x, pt.y, 2);
				steps.push_back(pt);
				dir = newDir;
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
			}
			else if (nextPtContent == 0)
			{
				pt = nextPt;
				map->setValue(pt.x, pt.y, 2);
				steps.push_back(pt);
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
			}
			else if (newDirectionTwoTimes(pt.x, pt.y).first != Direction::Unknown)
			{
				auto [newDir, newDirV2] = newDirectionTwoTimes(pt.x, pt.y);
				pt = step(pt.x, pt.y, newDir);
				map->setValue(pt.x, pt.y, 2);
				steps.push_back(pt);
				dir = newDirV2;
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
			}
			else if (map->allSidesUnavailable(pt.x, pt.y))
			{
				Point nearestZero = findNearestZero(pt.x, pt.y);
				std::vector<Point> roadBetweenPoints = moveBetweenPoints(pt.x, pt.y, nearestZero.x, nearestZero.y);
				size_t roadStepNum = 1;
				while (steps.back() != nearestZero)
				{
					nextPt = roadBetweenPoints[roadStepNum];
					if (map->getMap()[nextPt.y][nextPt.x] != 1)
					{
						pt = nextPt;
						steps.push_back(pt);
						roadStepNum++;
					}
					else
					{
						std::vector<Point> roadFromPtToZero;
						for (size_t i = roadStepNum + 1; i < roadBetweenPoints.size(); i++)
							roadFromPtToZero.push_back(roadBetweenPoints[i]);
						std::vector<Point> avoidenceRoad = avoidObstacle(pt.x, pt.y, roadFromPtToZero);
						for (const auto& point : avoidenceRoad)
							steps.push_back(point);
						std::vector<Point>::iterator it = std::find(roadBetweenPoints.begin(), roadBetweenPoints.end(), steps.back());
						roadStepNum = std::distance(roadBetweenPoints.begin(), it) + 1;
					}
				}
				pt = steps.back();
				map->setValue(pt.x, pt.y, 2);
			}
		}
	}
	return steps;
}

int Track::checkRightSide(const size_t x, const size_t y, const Direction dir)
{
	const size_t rows = map->getMap().size();
	const size_t columns = map->getMap()[0].size();
	/*if (x==0 || x==columns-1 || y == 0 || y == rows-1)
		throw std::invalid_argument("Robot is on the Wall");*/
	int content = 0;
	switch (dir)
	{
	case Direction::Right:
		content = map->getMap()[y + 1][x];
		break;
	case Direction::Left:
		content = map->getMap()[y - 1][x];
		break;
	case Direction::Up:
		content = map->getMap()[y][x + 1];
		break;
	case Direction::Down:
		content = map->getMap()[y][x - 1];
		break;
	}
	return content;
}

int Track::checkLeftSide(const size_t x, const size_t y, const Direction dir) {
	const size_t rows = map->getMap().size();
	const size_t columns = map->getMap()[0].size();
	/*if (x == 0 || x == columns - 1 || y == 0 || y == rows - 1)
		throw std::invalid_argument("Robot is on the Wall");*/
	int content = 0;
	switch (dir)
	{
	case Direction::Right:
		content = map->getMap()[y - 1][x];
		break;
	case Direction::Left:
		content = map->getMap()[y + 1][x];
		break;
	case Direction::Up:
		content = map->getMap()[y][x - 1];
		break;
	case Direction::Down:
		content = map->getMap()[y][x + 1];
		break;
	}
	return content;
}

Direction Track::newDirection(const size_t x, const size_t y) {
	if (map->getMap()[y][x + 1] == 0 && checkRightSide(x + 1, y, Direction::Right))
		return Direction::Right;
	else if (map->getMap()[y][x - 1] == 0 && checkRightSide(x - 1, y, Direction::Left))
		return Direction::Left;
	else if (map->getMap()[y - 1][x] == 0 && checkRightSide(x, y - 1, Direction::Up))
		return Direction::Up;
	else if (map->getMap()[y + 1][x] == 0 && checkRightSide(x, y + 1, Direction::Down))
		return Direction::Down;
	else
		return Direction::Unknown;
}

Direction Track::newDirectionExeption(const size_t x, const size_t y) {
	if (map->getMap()[y][x + 1] != 1 && checkRightSide(x + 1, y, Direction::Right) == 1)
		return Direction::Right;
	else if (map->getMap()[y][x - 1] != 1 && checkRightSide(x - 1, y, Direction::Left) == 1)
		return Direction::Left;
	else if (map->getMap()[y - 1][x] != 1 && checkRightSide(x, y - 1, Direction::Up) == 1)
		return Direction::Up;
	else if (map->getMap()[y + 1][x] != 1 && checkRightSide(x, y + 1, Direction::Down) == 1)
		return Direction::Down;
	else
		return Direction::Unknown;
}

Direction Track::newDirectionExeptionLeft(const size_t x, const size_t y) {
	if (map->getMap()[y][x + 1] != 1 && checkLeftSide(x + 1, y, Direction::Right) == 1)
		return Direction::Right;
	else if (map->getMap()[y][x - 1] != 1 && checkLeftSide(x - 1, y, Direction::Left) == 1)
		return Direction::Left;
	else if (map->getMap()[y - 1][x] != 1 && checkLeftSide(x, y - 1, Direction::Up) == 1)
		return Direction::Up;
	else if (map->getMap()[y + 1][x] != 1 && checkLeftSide(x, y + 1, Direction::Down) == 1)
		return Direction::Down;
	else
		return Direction::Unknown;
}

vector<Point> Track::moveBetweenPoints(const int x1, const int y1, const int x2, const int y2) {
	auto l = line(x1, y1, x2, y2);
	vector<Point> road;
	if (x1 < x2){
		if (y1 < y2){
			for (int y = y1; y <= y2; ++y){
				for (int x = x1; x <= x2; ++x){
					if (lineGoesThroughtArea(x, y, l))
					{
						Point p = { (size_t)x, (size_t)y };
						road.push_back(p);
					}
				}
			}
		}
		else{
			for (int y = y1; y >= y2; --y){
				for (int x = x1; x <= x2; ++x){
					if (lineGoesThroughtArea(x, y, l)){
						Point p = { (size_t)x, (size_t)y };
						road.push_back(p);
					}
				}
			}
		}
	}
	else if (x1 > x2){
		if (y1 < y2){
			for (int y = y1; y <= y2; ++y){
				for (int x = x1; x >= x2; --x){
					if (lineGoesThroughtArea(x, y, l)){
						Point p = { (size_t)x, (size_t)y };
						road.push_back(p);
					}
				}
			}
		}
		else{
			for (int y = y1; y >= y2; --y){
				for (int x = x1; x >= x2; --x){
					if (lineGoesThroughtArea(x, y, l)){
						Point p = { (size_t)x, (size_t)y };
						road.push_back(p);
					}
				}
			}
		}
	}
	else{
		if (y1 > y2){
			for (int y = y1; y >= y2; --y){
				Point p = { (size_t)x1, (size_t)y };
				road.push_back(p);
			}
		}
		else {
			for (int y = y1; y <= y2; ++y){
				Point p = { (size_t)x1, (size_t)y };
				road.push_back(p);
			}
		}
	}
	return road;
}

Point Track::findNearestZero(const size_t x1, const size_t y1) {
	auto rows = map->getMap().size();
	auto columns = map->getMap()[0].size();
	Point p1 = { x1, y1 }, bottomRight = { columns - 1, rows - 1 }, upperLeft = { 0, 0 };
	double minDis = distance(upperLeft, bottomRight), newDis;
	Point nearestPt = p1;
	Point pt;
	for (size_t y = 0; y < rows; ++y){
		for (size_t x = 0; x < columns; ++x){
			if (map->getMap()[y][x] == 0){
				pt = { x, y };
				newDis = distance(p1, pt);
				if (newDis < minDis){
					minDis = newDis;
					nearestPt = pt;
				}
			}
		}
	}
	return nearestPt;
}

pair<Direction, Direction> Track::newDirectionTwoTimes(const size_t x1, const size_t y1) {
	Direction newDir = Direction::Unknown, newDir2 = Direction::Unknown;
	if (map->getMap()[y1][x1 + 1] == 0) {
		newDir2 = newDirection(x1 + 1, y1);
		if (newDir2 != Direction::Unknown)
			return make_pair(Direction::Right, newDir2);
	}
	if (map->getMap()[y1][x1 - 1] == 0) {
		newDir2 = newDirection(x1 - 1, y1);
		if (newDir2 != Direction::Unknown)
			return make_pair(Direction::Left, newDir2);
	}
	if (map->getMap()[y1 - 1][x1] == 0) {
		newDir2 = newDirection(x1, y1 - 1);
		if (newDir2 != Direction::Unknown)
			return make_pair(Direction::Up, newDir2);
	}
	if (map->getMap()[y1 + 1][x1] == 0) {
		newDir2 = newDirection(x1, y1 + 1);
		if (newDir2 != Direction::Unknown)
			return make_pair(Direction::Down, newDir2);
	}
	return make_pair(newDir, newDir2);
}

pair<Direction, Direction> Track::newDirectionTwoTimesExeption(const size_t x1, const size_t y1) {
	Direction newDir = Direction::Unknown, newDir2 = Direction::Unknown;
	if (map->getMap()[y1][x1 + 1] != 1) {
		newDir2 = newDirectionExeption(x1 + 1, y1);
		if (newDir2 != Direction::Unknown)
			return std::make_pair(Direction::Right, newDir2);
	}
	if (map->getMap()[y1][x1 - 1] != 1) {
		newDir2 = newDirectionExeption(x1 - 1, y1);
		if (newDir2 != Direction::Unknown)
			return std::make_pair(Direction::Left, newDir2);
	}
	if (map->getMap()[y1 - 1][x1] != 1) {
		newDir2 = newDirectionExeption(x1, y1 - 1);
		if (newDir2 != Direction::Unknown)
			return std::make_pair(Direction::Up, newDir2);
	}
	if (map->getMap()[y1 + 1][x1] != 1) {
		newDir2 = newDirectionExeption(x1, y1 + 1);
		if (newDir2 != Direction::Unknown)
			return std::make_pair(Direction::Down, newDir2);
	}
	return std::make_pair(newDir, newDir2);
}
pair<Direction, Direction> Track::newDirectionTwoTimesExeptionLeft(const size_t x1, const size_t y1) {
	Direction newDir = Direction::Unknown, newDir2 = Direction::Unknown;
	if (map->getMap()[y1][x1 + 1] != 1) {
		newDir2 = newDirectionExeptionLeft(x1 + 1, y1);
		if (newDir2 != Direction::Unknown)
			return std::make_pair(Direction::Right, newDir2);
	}
	if (map->getMap()[y1][x1 - 1] != 1) {
		newDir2 = newDirectionExeptionLeft(x1 - 1, y1);
		if (newDir2 != Direction::Unknown)
			return std::make_pair(Direction::Left, newDir2);
	}
	if (map->getMap()[y1 - 1][x1] != 1) {
		newDir2 = newDirectionExeptionLeft(x1, y1 - 1);
		if (newDir2 != Direction::Unknown)
			return std::make_pair(Direction::Up, newDir2);
	}
	if (map->getMap()[y1 + 1][x1] != 1) {
		newDir2 = newDirectionExeptionLeft(x1, y1 + 1);
		if (newDir2 != Direction::Unknown)
			return std::make_pair(Direction::Down, newDir2);
	}
	return std::make_pair(newDir, newDir2);
}

vector<Point> Track::avoidObstacleFromRight(const size_t x1, const size_t y1, vector<Point> orginalRoad) {
	std::vector<Point> roadFromRightSide;
	Point pt = { x1 , y1 }, nextPt = {};
	int nextPtRightSide = 2, nextPtContent = 1;
	Direction dir = Direction::Unknown;

	while (std::find(orginalRoad.begin(), orginalRoad.end(), pt) == orginalRoad.end())
	{
		if (nextPtRightSide == 1 && nextPtContent == 2)
		{
			pt = nextPt;
			roadFromRightSide.push_back(pt);
			nextPt = step(pt.x, pt.y, dir);
			nextPtContent = map->getMap()[nextPt.y][nextPt.x];
			nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
		}
		else
		{
			Direction newDir = newDirectionExeption(pt.x, pt.y);
			if (newDir != Direction::Unknown)
			{
				pt = step(pt.x, pt.y, newDir);
				roadFromRightSide.push_back(pt);
				dir = newDir;
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
			}
			else if (nextPtContent == 2)
			{
				pt = nextPt;
				roadFromRightSide.push_back(pt);
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
			}
			else if (newDirectionTwoTimesExeption(pt.x, pt.y).first != Direction::Unknown)
			{
				auto [newDir, newDir2] = newDirectionTwoTimesExeption(pt.x, pt.y);
				pt = step(pt.x, pt.y, newDir);
				roadFromRightSide.push_back(pt);
				dir = newDir2;
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtRightSide = checkRightSide(nextPt.x, nextPt.y, dir);
			}
		}
	}
	return roadFromRightSide;
}
vector<Point> Track::avoidObstacleFromLeft(const size_t x1, const size_t y1, vector<Point> orginalRoad) {
	std::vector<Point> roadFromLeftSide;
	Point pt = { x1 , y1 }, nextPt = {};
	int nextPtLeftSide = 0, nextPtContent = 1;
	Direction dir = Direction::Unknown;

	while (std::find(orginalRoad.begin(), orginalRoad.end(), pt) == orginalRoad.end())
	{
		if (nextPtLeftSide == 1 && nextPtContent == 2)
		{
			pt = nextPt;
			roadFromLeftSide.push_back(pt);
			nextPt = step(pt.x, pt.y, dir);
			nextPtContent = map->getMap()[nextPt.y][nextPt.x];
			nextPtLeftSide = checkLeftSide(nextPt.x, nextPt.y, dir);
		}
		else
		{
			Direction newDir = newDirectionExeptionLeft(pt.x, pt.y);
			if (newDir != Direction::Unknown)
			{
				pt = step(pt.x, pt.y, newDir);
				roadFromLeftSide.push_back(pt);
				dir = newDir;
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtLeftSide = checkLeftSide(nextPt.x, nextPt.y, dir);
			}
			else if (nextPtContent == 2)
			{
				pt = nextPt;
				roadFromLeftSide.push_back(pt);
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtLeftSide = checkLeftSide(nextPt.x, nextPt.y, dir);
			}
			else if (newDirectionTwoTimesExeptionLeft(pt.x, pt.y).first != Direction::Unknown)
			{
				auto [newDir, newDir2] = newDirectionTwoTimesExeptionLeft(pt.x, pt.y);
				pt = step(pt.x, pt.y, newDir);
				roadFromLeftSide.push_back(pt);
				dir = newDir2;
				nextPt = step(pt.x, pt.y, dir);
				nextPtContent = map->getMap()[nextPt.y][nextPt.x];
				nextPtLeftSide = checkLeftSide(nextPt.x, nextPt.y, dir);
			}
		}
	}
	return roadFromLeftSide;
}

vector<Point> Track::avoidObstacle(const size_t x1, const size_t y1, vector<Point> orginalRoad) {
	//Próbujê omin¹æ z 2 stron i patrze która droga jest krótsza
	std::vector<Point> roadFromRightSide = avoidObstacleFromRight(x1, y1, orginalRoad);
	std::vector<Point> roadFromLeftSide = avoidObstacleFromLeft(x1, y1, orginalRoad);
	if (roadFromLeftSide.size() < roadFromRightSide.size())
		return roadFromLeftSide;
	else
		return roadFromRightSide;
}

Point Track::step(size_t x0, size_t y0, Direction dir) noexcept {
	Point p = {};
	switch (dir)
	{
	case Direction::Right:
		p = { x0 + 1, y0 };
		break;
	case Direction::Left:
		p = { x0 - 1, y0 };
		break;
	case Direction::Up:
		p = { x0, y0 - 1 };
		break;
	case Direction::Down:
		p = { x0, y0 + 1 };
		break;
	}
	return p;
}

double Track::lineInX(const int x, const pair<double, double> line) {
	auto [a, b] = line;
	return a * (double)x + b;
}
pair<double, double> Track::line(const int x1, const int y1, const int x2, const int y2) {
	double dy = (double)y2 - (double)y1; double dx = (double)x2 - (double)x1;
	double a = dy / dx;
	double b = (double)y1 - a * (double)x1;
	return std::make_pair(a, b);
}

bool Track::lineGoesThroughtArea(const int x, const int y, const pair<double, double> line) {
	auto [a, b] = line;
	if ((y <= lineInX(x, line) && lineInX(x, line) < (double)y + 1) ||
		(y <= lineInX(x + 1, line) && lineInX(x + 1, line) < (double)y + 1) ||
		(x < (double)((double)y - b) / a && (double)((double)y - b) / a < (double)x + 1) ||
		(x < (double)((double)y + 1 - b) / a && (double)((double)y + 1 - b) / a < (double)x + 1))
		return true;
	else
		return false;
}

double Track::distance(Point p1, Point p2) {
	double dx = (double)p1.x - (double)p2.x;
	double dy = (double)p1.y - (double)p2.y;
	return sqrt(std::pow(dx, 2) + std::pow(dy, 2));
}

bool operator==(vector<Point>& tr1, vector<Point>& tr2) {
	if (tr2.size() != tr1.size()) return false;
	for (int i = 0; i < tr1.size(); ++i)
		if (tr2[i].x != tr1[i].x || tr2[i].y != tr1[i].y)
			return false;
	return true;
}

bool operator==(Point& p1, Point& p2) {
	if (p1.x != p2.x || p1.y != p2.y) return false;
	return true;
}

bool operator!=(Point& p1, Point& p2) {
	if (p1.x != p2.x || p1.y != p2.y) return true;
	return false;
}