#include "CppUnitTest.h"
#include "..\CleaningRobot\Robot.h"
#include "..\CleaningRobot\Map.h"
#include "..\CleaningRobot\Track.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TrackTests
{
	TEST_CLASS(TrackTests)
	{
	public:

		TEST_METHOD(Constructor)
		{
			Assert::IsTrue(1 == 1);
		};

        TEST_METHOD(distance)
        {
            my_array map2d;
            Map map(map2d);
            size_t x0 = 1, y0 = 1;
            Track t(x0, y0, &map);
            Point p1 = { 0, 0 }; Point p2 = { 3, 4 };
            Assert::AreEqual(5.0, t.distance(p1, p2), 0.01);
            Point p3 = { 17, 29 }; Point p4 = { 12, 17 };
            Assert::AreEqual(13.0, t.distance(p3, p4), 0.01);
        };

        TEST_METHOD(findNearestZero)
        {
            my_array map2d;
            Map map(map2d);
            map.addObstacle(0, 0, map.getMap()[0].size() - 1, map.getMap().size() - 1);
            size_t x0 = 1, y0 = 1;
            Track t(x0, y0, &map);
            map.setValue(5, 7, 0); map.setValue(20, 10, 0);
            auto n0 = t.findNearestZero(x0, y0);
            Assert::AreEqual(n0.x, (size_t)5);
            Assert::AreEqual(n0.y, (size_t)7);
            auto n00 = t.findNearestZero(15, 10);
            Assert::AreEqual(n00.x, (size_t)20);
            Assert::AreEqual(n00.y, (size_t)10);
        };

        TEST_METHOD(lineGoesThroughtArea)
        {
            my_array map2d;
            Map map(map2d);
            size_t x0 = 1, y0 = 1;
            Track t(x0, y0, &map);
            std::pair<double, double> l = std::make_pair(1.0, 0.0);
            Assert::IsTrue(t.lineGoesThroughtArea(0, 0, l));
            Assert::IsTrue(t.lineGoesThroughtArea(0, 1, l));
            Assert::IsTrue(t.lineGoesThroughtArea(5, 5, l));
            Assert::IsTrue(t.lineGoesThroughtArea(5, 6, l));
            std::pair<double, double> l1 = std::make_pair(-2.5, 13.0);
            Assert::IsTrue(t.lineGoesThroughtArea(0, 13, l1));
            Assert::IsTrue(t.lineGoesThroughtArea(1, 10, l1));
            Assert::IsTrue(t.lineGoesThroughtArea(2, 8, l1));
            Assert::IsTrue(t.lineGoesThroughtArea(4, 3, l1));
        };

        TEST_METHOD(line)
        {
            my_array map2d;
            Map map(map2d);
            size_t x0 = 1, y0 = 1;
            Track t(x0, y0, &map);
            auto [a1, b1] = t.line(0, 0, 5, 5);
            Assert::AreEqual(a1, 1.0, 0.01);
            Assert::AreEqual(b1, 0.0, 0.01);
            auto [a2, b2] = t.line(5, 5, 0, 0);
            Assert::AreEqual(a2, 1.0, 0.01);
            Assert::AreEqual(b2, 0.0, 0.01);
            auto [a3, b3] = t.line(1, 5, 31, 26);
            Assert::AreEqual(a3, 0.7, 0.01);
            Assert::AreEqual(b3, 4.3, 0.01);
        };

        TEST_METHOD(step)
        {
            my_array map2d;
            Map map(map2d);
            size_t x0 = 1, y0 = 1;
            Track t(x0, y0, &map);
            Point p1 = t.step(x0, y0, Direction::Right);
            Assert::AreEqual(p1.x, (size_t)2);
            Assert::AreEqual(p1.y, (size_t)1);



            Point p2 = t.step(p1.x, p1.y, Direction::Down);
            Assert::AreEqual(p2.x, (size_t)2);
            Assert::AreEqual(p2.y, (size_t)2);



            Point p3 = t.step(p2.x, p2.y, Direction::Left);
            Assert::AreEqual(p3.x, (size_t)1);
            Assert::AreEqual(p3.y, (size_t)2);



            Point p4 = t.step(p3.x, p3.y, Direction::Up);
            Assert::AreEqual(p4.x, (size_t)1);
            Assert::AreEqual(p4.y, (size_t)1);
        };

        TEST_METHOD(moveBetweenPoints)
        {
            my_array map2d;
            Map map(map2d);
            size_t x0 = 1, y0 = 1;
            Track t(x0, y0, &map);
            std::vector<Point> pts = t.moveBetweenPoints(x0, y0, 3, 3);
            std::vector<Point> ptsA = { {1, 1}, {1, 2}, {2, 2}, {2, 3}, {3, 3} };
            for (size_t i = 0; i < pts.size(); ++i) {
                Assert::AreEqual(pts[i].x, ptsA[i].x);
                Assert::AreEqual(pts[i].y, ptsA[i].y);
            }
            std::vector<Point> pts1 = t.moveBetweenPoints(2, 6, 0, 0);
            std::vector<Point> ptsA1 = { {2, 6}, {1, 6}, {1, 5}, {1, 4}, {1, 3}, {0, 3}, {0, 2}, {0, 1}, {0, 0} };
            for (size_t j = 0; j < pts1.size(); ++j) {
                Assert::AreEqual(pts1[j].x, ptsA1[j].x);
                Assert::AreEqual(pts1[j].y, ptsA1[j].y);
            }
        };

        TEST_METHOD(checkRightSide)
        {
            my_array map2d;
            Map map(map2d);
            size_t x0 = 1, y0 = 1;
            Track t(x0, y0, &map);
            map.setValue(1, 0, 2); map.setValue(2, 1, 1);
            map.setValue(1, 2, 1); map.setValue(0, 1, 2);
            Assert::AreEqual(t.checkRightSide(x0, x0, Direction::Right), 1);
            Assert::AreEqual(t.checkRightSide(x0, x0, Direction::Left), 2);
            Assert::AreEqual(t.checkRightSide(x0, x0, Direction::Up), 1);
            Assert::AreEqual(t.checkRightSide(x0, x0, Direction::Down), 2);
        }
        TEST_METHOD(checkLeftSide)
        {
            my_array map2d;
            Map map(map2d);
            size_t x0 = 1, y0 = 1;
            Track t(x0, y0, &map);
            map.setValue(1, 0, 2); map.setValue(2, 1, 1);
            map.setValue(1, 2, 1); map.setValue(0, 1, 2);
            Assert::AreEqual(t.checkLeftSide(x0, x0, Direction::Right), 2);
            Assert::AreEqual(t.checkLeftSide(x0, x0, Direction::Left), 1);
            Assert::AreEqual(t.checkLeftSide(x0, x0, Direction::Up), 2);
            Assert::AreEqual(t.checkLeftSide(x0, x0, Direction::Down), 1);
        };
	};
}
