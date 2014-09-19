#include <CppUTest/TestHarness.h>
#include "../src/CollisionDetector.h"
#include "../src/objectcontainer.h"
#include "../src/Block.h"

TEST_GROUP( CollisionDetectorTests )
{
	ObjectContainer* container;
	CollisionDetector* detector;

		Block* c1;
		Block* c2;
		Block* c3;
		Block* c4;

	void setup()
	{
		container = new ObjectContainer();
		detector = new CollisionDetector(container);

		c1 = new Block(100, 100);
		c2 = new Block(200, 100);
		c3 = new Block(200, 200);
		c4 = new Block(105, 105);

		container->registerObject(c1);
		container->registerObject(c2);
		container->registerObject(c3);
		container->registerObject(c4);
	}

	void teardown()
	{
		delete detector;
		delete container;
	}
};

TEST( CollisionDetectorTests, TestSingleCollision )
{
	CHECK( detector->hasCollided(c1, c4) );
	CHECK( !detector->hasCollided(c1, c2) );
}

TEST( CollisionDetectorTests, TestMultiCollisions )
{
	CHECK( detector->checkForCollisions( c4 ) );
	CHECK( !detector->checkForCollisions( c2 ) );
}
