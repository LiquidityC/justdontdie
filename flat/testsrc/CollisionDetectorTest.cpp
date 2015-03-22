#include "catch.hpp"
#include "EntityImpl.h"
#include "../src/CollisionDetector.h"
#include "../src/EntityContainer.h"

TEST_CASE( "CollisionDetectorTests", "[collision]" )
{
	flat2d::EntityContainer* container;
	flat2d::CollisionDetector* detector;

	flat2d::Entity* c1;
	flat2d::Entity* c2;
	flat2d::Entity* c3;
	flat2d::Entity* c4;

	container = new flat2d::EntityContainer();
	detector = new flat2d::CollisionDetector(container);

	c1 = new EntityImpl(100, 100);
	c2 = new EntityImpl(200, 100);
	c3 = new EntityImpl(200, 200);
	c4 = new EntityImpl(105, 105);

	container->registerObject(c1);
	container->registerObject(c2);
	container->registerObject(c3);
	container->registerObject(c4);

	SECTION( "Collision detection", "[collisions]" )
	{
		REQUIRE( detector->hasCollided(c1, c4) );
		REQUIRE( !detector->hasCollided(c1, c2) );
	}

	SECTION( "Multi collisions", "[collisions]" )
	{
		flat2d::Entity* o = detector->checkForCollisions( c4 );
		REQUIRE( o );
		REQUIRE( *c1 == *o );

		REQUIRE( !detector->checkForCollisions( c2 ) );
	}

	SECTION( "Colliders", "[collisions]" )
	{
		SDL_Rect colliderBox = { 0, 0, 1, 1 };
		c1->setColliderBox(colliderBox);

		REQUIRE( !detector->hasCollided(c1, c4) );
	}

	delete detector;
	delete container;
}

