#include <flat/flat.h>
#include "catch.hpp"
#include "../src/Soldier.h"
#include "../src/Rocket.h"

TEST_CASE ( "Test the soldier", "[soldier]" )
{
	flat2d::ObjectContainer *container;
	flat2d::CollisionDetector *detector;

	Soldier *soldier;

	container = new flat2d::ObjectContainer();
	detector = new flat2d::CollisionDetector(container);

	soldier = new Soldier(200, 200);
	container->registerObject(soldier);

	// Future sections here

	delete detector;
	delete container;
}
