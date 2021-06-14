#include "DrawnMap.h"
#include "Robot.h"

DrawnMap::DrawnMap(Map* map2d, string name) {
	this->size_x = map2d->getColumns();
	this->size_y = map2d->getRows();
	this->map2d = map2d;
	my_window.create(VideoMode(size_x * scale, size_y * scale), name);
}

void DrawnMap::drawObstacles() {
	RectangleShape rectangle(Vector2f(1 * scale, 1 * scale));
	rectangle.setFillColor(Color(175, 180, 240));

	for (size_t row = 0; row < size_y; ++row)
	{
		for (size_t column = 0; column < size_x; ++column)
		{
			if (map2d->getMap()[row][column] == 1)
			{
				rectangle.setPosition(column * scale, row * scale);
				my_window.draw(rectangle);
			}
		}
	}
}

void DrawnMap::drawTrace(vector<pair<int, int>>& coord_for_draw) {
	RectangleShape small_rectangle(Vector2f(1 * scale, 1 * scale));
	small_rectangle.setFillColor(Color(250, 250, 250));
	for (auto item : coord_for_draw) {
		small_rectangle.setPosition(item.first * scale, item.second * scale);
		my_window.draw(small_rectangle);
	}
}

void DrawnMap::drawRobot(int& x, int& y) {
	CircleShape circle(0.5 * scale);
	circle.setFillColor(Color(250, 0, 250));
	circle.setPosition(x * scale, y * scale);
	my_window.draw(circle);
}

vector<Point> DrawnMap::getTrack(const size_t x0, const size_t y0, Map* map2d) {
	Track track(x0, y0, map2d);
	vector<Point> road = track.getPoints();
	return road;
}

void DrawnMap::loop(int& x, int& y) {
	float timer = 0, delay = 0.1;
	Clock clock;
	vector<Point> trace = getTrack(x, y, map2d);
	int i = 0;
	x = trace[i].x;
	y = trace[i].y;
	vector<pair<int, int>> coord_for_draw = { make_pair(x, y) };
	Robot robot(x,y);
	while (my_window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		Event event;
		while (my_window.pollEvent(event)){
			if (event.type == Event::Closed) {
				my_window.close();
			}
		}
		if (timer > delay & i < trace.size()-1){
			i += 1;
			x = trace[i].x;
			y = trace[i].y;
			coord_for_draw.push_back({ x, y });
			timer = 0;
		}

		my_window.clear(Color(100, 100, 100));

		drawObstacles();
		drawTrace(coord_for_draw);
		robot.setPosition(x,y);
		int x0 = robot.getX();
		int y0 = robot.getY();
		drawRobot(x0, y0);
		my_window.display();
	}
}

int main()
{
	std::array<std::array<int, COLUMNS>, ROWS> map2d = { 0 };
	cout <<"y"<< map2d.size() << " x" << map2d[0].size();
	size_t started_x = 1, started_y = 1;
	int x = started_x, y = started_y;
	


	Map m1(map2d);
	m1.addWalls();
	m1.addObstacle(2, 3, 9, 4);
	DrawnMap dm(&m1, "Testik");
	dm.loop(x, y);
}
