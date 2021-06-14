#include "CppUnitTest.h"
#include "..\CleaningRobot\Robot.h"
#include "..\CleaningRobot\Map.h"
#include "..\CleaningRobot\Track.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RobotTests
{
	TEST_CLASS(RobotTests)
	{
	public:
		
		TEST_METHOD(Constructor)
		{
			int x = 5, y = 6;
			Robot r1(x, y);
			Assert::AreEqual(r1.getX(), x);
			Assert::AreEqual(r1.getY(), y);
		}

		TEST_METHOD(getX)
		{
			int x = 2, y = 3;
			Robot r1(x, y);
			Assert::AreEqual(r1.getX(), x);
		}

		TEST_METHOD(getY)
		{
			int x = 2, y = 3;
			Robot r1(x, y);
			Assert::AreEqual(r1.getY(), y);
		}

		TEST_METHOD(setX)
		{
			int x = 2, y = 3, x2 = 4;
			Robot r1(x, y);
			r1.setX(x2);
			Assert::AreEqual(r1.getX(), x2);
		}

		TEST_METHOD(setY)
		{
			int x = 2, y = 3, y2 = 4;
			Robot r1(x, y);
			r1.setY(y2);
			Assert::AreEqual(r1.getY(), y2);
		}

		TEST_METHOD(setPosition)
		{
			int x = 2, y = 3, x2 = 5, y2 = 4;
			Robot r1(x, y);
			r1.setPosition(x2, y2);
			Assert::AreEqual(r1.getX(), x2);
			Assert::AreEqual(r1.getY(), y2);
		}

	};
}
