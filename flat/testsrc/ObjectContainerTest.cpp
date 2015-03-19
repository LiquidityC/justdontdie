#include "GameObjectImpl.h"
#include "catch.hpp"
#include "../src/ObjectContainer.h"
#include "../src/GameData.h"
#include "../src/CollisionDetector.h"
#include "../src/Mixer.h"

TEST_CASE( "Test object register", "[objectcontainer]" )
{
	flat2d::GameObject* c = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer container;

	REQUIRE( 0 == container.getObjectCount() );
	container.registerObject(c);
	REQUIRE( 1 == container.getObjectCount() );
	container.registerObject(c);
	REQUIRE( 1 == container.getObjectCount() );
	container.unregisterObject(c);
	REQUIRE( 0 == container.getObjectCount() );

	delete c;
}

TEST_CASE( "Test container destructor", "[objectcontainer]" )
{
	flat2d::GameObject* c = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer* container = new flat2d::ObjectContainer();

	REQUIRE( 0 == container->getObjectCount() );

	container->registerObject(c);

	REQUIRE( 1 == container->getObjectCount() );

	container->registerObject(c);

	REQUIRE( 1 == container->getObjectCount() );

	delete container;
}

TEST_CASE( "Test unregister", "[objectcontainer]" )
{
	flat2d::GameObject* c = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer container;

	container.unregisterObject(c);

	delete c;
}

TEST_CASE( "Test layers", "[objectcontainer]" )
{
	flat2d::GameObject* c1 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c2 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c3 = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer container;
	
	REQUIRE( 0 == container.getObjectCountFor(0) );

	container.addLayer(0);
	container.addLayer(0);
	container.addLayer(1);

	container.registerObject(c1);

	REQUIRE( 1 == container.getObjectCount() );
	REQUIRE( 1 == container.getObjectCountFor(flat2d::ObjectContainer::DEFAULT_LAYER) );

	container.registerObject(c2, 0);
	container.registerObject(c2, 1);

	REQUIRE( 2 == container.getObjectCount() );
	REQUIRE( 1 == container.getObjectCountFor(0) );
	REQUIRE( 0 == container.getObjectCountFor(1) );

	container.registerObject(c3, 1);

	REQUIRE( 1 == container.getObjectCountFor(1) );
	REQUIRE( 3 == container.getObjectCount() );

	container.unregisterObject(c2);

	REQUIRE( 0 == container.getObjectCountFor(0) );
	REQUIRE( 2 == container.getObjectCount() );

	delete c2;
}

TEST_CASE( "Test clearing layers", "[objectcontainer]" )
{
	flat2d::GameObject* c1 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c2 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c3 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c4 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c5 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c6 = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer container;

	unsigned int backLayer = 0;
	unsigned int frontLayer = 1;

	container.addLayer(backLayer);
	container.addLayer(frontLayer);

	container.registerObject(c1, frontLayer);
	container.registerObject(c2, frontLayer);
	container.registerObject(c3, frontLayer);
	container.registerObject(c4, backLayer);
	container.registerObject(c5, backLayer);
	container.registerObject(c6, backLayer);

	REQUIRE ( 6 == container.getObjectCount() );
	REQUIRE ( 3 == container.getObjectCountFor(frontLayer) );
	REQUIRE ( 3 == container.getObjectCountFor(backLayer) );

	container.unregisterAllObjectsFor(frontLayer);

	REQUIRE ( 3 == container.getObjectCount() );
	REQUIRE ( 0 == container.getObjectCountFor(frontLayer) );
	REQUIRE ( 3 == container.getObjectCountFor(backLayer) );
}

TEST_CASE( "Test auto cleaning", "[objectcontainer]" )
{
	flat2d::GameObject* c1 = new GameObjectImpl(100, 100);
	GameObjectImpl* c2 = new GameObjectImpl(100, 100);

	flat2d::ObjectContainer container;
	flat2d::CollisionDetector detector(&container);
	flat2d::Mixer mixer;
	flat2d::GameData gameData(&container, &detector, &mixer, (flat2d::RenderData*) nullptr,
			(flat2d::DeltatimeMonitor*) nullptr);

	container.registerObject(c1);
	container.registerObject(c2);

	REQUIRE ( 2 == container.getObjectCount() );

	c2->setDead(true);
	container.preHandleObjects(&gameData);

	REQUIRE ( 1 == container.getObjectCount() );
}

TEST_CASE( "Test partition handling", "[objectcontainer]" )
{
	flat2d::ObjectContainer container;
	flat2d::GameObject* o = new GameObjectImpl(45, 45);

	container.setSpatialPartitionDimension(50);
	container.registerObject(o);

	REQUIRE( 4 == o->getLocationProperty().getParents().size() );

	o->getLocationProperty().incrementXpos(10);

	REQUIRE( 2 == o->getLocationProperty().getParents().size() );
}

TEST_CASE( "Test spatial partitions", "[objectcontainer]" )
{
	flat2d::ObjectContainer container;

	flat2d::GameObject* o1 = new GameObjectImpl(50, 50);
	flat2d::GameObject* o2 = new GameObjectImpl(150, 150);
	flat2d::GameObject* o3 = new GameObjectImpl(550, 550);
	flat2d::GameObject* o4 = new GameObjectImpl(75, 75);
	flat2d::GameObject* o5 = new GameObjectImpl(1095, 1095);

	container.registerObject(o1);
	REQUIRE ( 1 == container.getSpatialPartitionCount() );

	container.registerObject(o4);
	REQUIRE ( 1 == container.getSpatialPartitionCount() );

	container.registerObject(o2);
	REQUIRE ( 2 == container.getSpatialPartitionCount() );

	container.registerObject(o3);
	REQUIRE ( 3 == container.getSpatialPartitionCount() );

	container.registerObject(o5);
	REQUIRE ( 7 == container.getSpatialPartitionCount() );
}
