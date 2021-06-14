#include "CppUnitTest.h"
#include "..\CleaningRobot\Robot.h"
#include "..\CleaningRobot\Map.h"
#include "..\CleaningRobot\Track.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MapTests
{
	TEST_CLASS(MapTests)
	{
	public:

		TEST_METHOD(Constructor)
		{
			my_array map2d;
			Map m1(map2d);
			Assert::IsTrue(m1.getRows() == ROWS);
			Assert::IsTrue(m1.getColumns() == COLUMNS);
			Assert::IsTrue(m1.getMap() == map2d);
		};

		TEST_METHOD(getRows)
		{
			my_array map2d = { 1 };
			Map m1(map2d);
			Assert::IsTrue(m1.getRows() == ROWS);
		};

		TEST_METHOD(getColumns)
		{
			my_array map2d = { 1 };
			Map m1(map2d);
			Assert::IsTrue(m1.getColumns() == COLUMNS);
		};

		TEST_METHOD(getMap)
		{
			my_array map2d = { 1 };
			Map m1(map2d);
			Assert::IsTrue(m1.getMap() == map2d);
		};

		TEST_METHOD(setValue)
		{
			my_array map2d = { 1 };
			Map m1(map2d);
			int value = 20;
			m1.setValue(2, 2, value);
			Assert::IsTrue(m1.getMap()[2][2] == value);
		};

		TEST_METHOD(addWalls)
		{
			my_array map2d;
			Map m1(map2d);
			m1.addWalls();
			for (int x = 0; x < COLUMNS; ++x) {
				Assert::IsTrue(m1.getMap()[0][x] == 1);
				Assert::IsTrue(m1.getMap()[ROWS - 1][x] == 1);
			}
			for (int y = 1; y < ROWS - 1; ++y) {
				Assert::IsTrue(m1.getMap()[y][0] == 1);
				Assert::IsTrue(m1.getMap()[y][COLUMNS - 1] == 1);
			}
		};

		TEST_METHOD(addObstacle)
		{
			my_array map2d;
			Map m1(map2d);
			const size_t x1 = 2, y1 = 3, x2 = 6, y2 = 4;
			m1.addObstacle(x1, y1, x2, y2);
			for (size_t i = y1; i <= y2; ++i) {
				for (size_t j = x1; j <= x2; ++j) {
					Assert::IsTrue(m1.getMap()[i][j] = 1);
				}
			}
		};

		TEST_METHOD(checkIfAllCleanedWithAllZero)
		{
			my_array map2d = { 0 };
			Map m1(map2d);
			Assert::IsTrue(!m1.checkIfAllCleaned());
		};

		TEST_METHOD(checkIfAllCleanedWithOneZero)
		{
			my_array map2d = { 1 };
			Map m1(map2d);
			m1.setValue(2, 2, 0);
;			Assert::IsTrue(!m1.checkIfAllCleaned());
		};
	};
}
