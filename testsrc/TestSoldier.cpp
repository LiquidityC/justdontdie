#include <CppUTest/TestHarness.h>
#include <flat/flat.h>
#include "../src/Soldier.h"
#include "../src/Rocket.h"

TEST_GROUP ( SoldierTests )
{
	flat2d::ObjectContainer *container;
	flat2d::CollisionDetector *detector;

	Soldier *soldier;

	void setup()
	{
		container = new flat2d::ObjectContainer();
		detector = new flat2d::CollisionDetector(container);

		soldier = new Soldier(NULL, 200, 200);
		container->registerObject(soldier);
	}

	void teardown()
	{
		delete detector;
		delete container;
	}
};

/* This isn't testing what I intended to test */
IGNORE_TEST( SoldierTests, TestRocketCollision )
{

	Rocket *r1 = new Rocket( 200, 200 );
	Rocket *r2 = new Rocket( 200, 200 );

	CHECK ( !detector->hasCollided(soldier, r1) );
	CHECK ( detector->hasCollided(soldier, r2) );

	soldier->setGhostMode(true);

	CHECK ( detector->hasCollided(soldier, r1) );
	CHECK ( !detector->hasCollided(soldier, r2) );
}
