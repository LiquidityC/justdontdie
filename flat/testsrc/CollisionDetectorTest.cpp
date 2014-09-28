#include <CppUTest/TestHarness.h>
#include "GameObjectImpl.h"
#include "../src/CollisionDetector.h"
#include "../src/ObjectContainer.h"

TEST_GROUP( CollisionDetectorTests )
{
	flat2d::ObjectContainer* container;
	flat2d::CollisionDetector* detector;

	flat2d::GameObject* c1;
	flat2d::GameObject* c2;
	flat2d::GameObject* c3;
	flat2d::GameObject* c4;

	void setup()
	{
		container = new flat2d::ObjectContainer();
		detector = new flat2d::CollisionDetector(container);

		c1 = new GameObjectImpl(100, 100);
		c2 = new GameObjectImpl(200, 100);
		c3 = new GameObjectImpl(200, 200);
		c4 = new GameObjectImpl(105, 105);

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
	flat2d::GameObject* o = detector->checkForCollisions( c4 );
	CHECK( o );
	CHECK( *c1 == *o );

	CHECK( !detector->checkForCollisions( c2 ) );
}
