#include "catch.hpp"
#include "GameObjectImpl.h"
#include "../src/CollisionDetector.h"
#include "../src/ObjectContainer.h"

TEST_CASE( "CollisionDetectorTests", "[collision]" )
{
	flat2d::ObjectContainer* container;
	flat2d::CollisionDetector* detector;

	flat2d::GameObject* c1;
	flat2d::GameObject* c2;
	flat2d::GameObject* c3;
	flat2d::GameObject* c4;

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

	SECTION( "Collision detection", "[collisions]" )
	{
		REQUIRE( detector->hasCollided(c1, c4) );
		REQUIRE( !detector->hasCollided(c1, c2) );
	}

	SECTION( "Multi collisions", "[collisions]" )
	{
		flat2d::GameObject* o = detector->checkForCollisions( c4 );
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

