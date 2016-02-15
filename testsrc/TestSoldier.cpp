#include <flat/flat.h>
#include "catch.hpp"
#include "../src/Soldier.h"
#include "../src/npcs/Rocket.h"

TEST_CASE ( "Test the soldier", "[soldier]" )
{
	flat2d::DeltatimeMonitor *dtm;
	flat2d::EntityContainer *container;
	flat2d::CollisionDetector *detector;

	Soldier *soldier;

	dtm = new flat2d::DeltatimeMonitor();
	container = new flat2d::EntityContainer(dtm);
	detector = new flat2d::CollisionDetector(container, dtm);

	soldier = new Soldier(200, 200);
	container->registerObject(soldier);

	// Future sections here

	delete detector;
	delete container;
	delete dtm;
}
