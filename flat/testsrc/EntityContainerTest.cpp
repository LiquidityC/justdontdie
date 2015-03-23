#include "EntityImpl.h"
#include "catch.hpp"
#include "../src/EntityContainer.h"
#include "../src/EntityProperties.h"
#include "../src/GameData.h"
#include "../src/CollisionDetector.h"
#include "../src/Mixer.h"

TEST_CASE( "Test object register", "[objectcontainer]" )
{
	flat2d::Entity* c = new EntityImpl(100, 100);
	flat2d::EntityContainer container;

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
	flat2d::Entity* c = new EntityImpl(100, 100);
	flat2d::EntityContainer* container = new flat2d::EntityContainer();

	REQUIRE( 0 == container->getObjectCount() );

	container->registerObject(c);

	REQUIRE( 1 == container->getObjectCount() );

	container->registerObject(c);

	REQUIRE( 1 == container->getObjectCount() );

	delete container;
}

TEST_CASE( "Test unregister", "[objectcontainer]" )
{
	flat2d::Entity* c = new EntityImpl(100, 100);
	flat2d::EntityContainer container;

	container.unregisterObject(c);

	delete c;
}

TEST_CASE( "Test layers", "[objectcontainer]" )
{
	flat2d::Entity* c1 = new EntityImpl(100, 100);
	flat2d::Entity* c2 = new EntityImpl(100, 100);
	flat2d::Entity* c3 = new EntityImpl(100, 100);
	flat2d::EntityContainer container;
	
	REQUIRE( 0 == container.getObjectCountFor(0) );

	container.addLayer(0);
	container.addLayer(0);
	container.addLayer(1);

	container.registerObject(c1);

	REQUIRE( 1 == container.getObjectCount() );
	REQUIRE( 1 == container.getObjectCountFor(flat2d::EntityContainer::DEFAULT_LAYER) );

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
	flat2d::Entity* c1 = new EntityImpl(100, 100);
	flat2d::Entity* c2 = new EntityImpl(100, 100);
	flat2d::Entity* c3 = new EntityImpl(100, 100);
	flat2d::Entity* c4 = new EntityImpl(100, 100);
	flat2d::Entity* c5 = new EntityImpl(100, 100);
	flat2d::Entity* c6 = new EntityImpl(100, 100);
	flat2d::EntityContainer container;

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
	flat2d::Entity* c1 = new EntityImpl(100, 100);
	EntityImpl* c2 = new EntityImpl(100, 100);

	flat2d::EntityContainer container;
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
	flat2d::EntityContainer container;
	flat2d::Entity* o = new EntityImpl(45, 45);

	container.setSpatialPartitionDimension(50);
	container.registerObject(o);

	REQUIRE( 4 == o->getEntityProperties().getCurrentAreas().size() );

	o->getEntityProperties().incrementXpos(10);

	REQUIRE( 2 == o->getEntityProperties().getCurrentAreas().size() );
}

TEST_CASE( "Test spatial partitions", "[objectcontainer]" )
{
	flat2d::EntityContainer container;

	flat2d::Entity* o1 = new EntityImpl(50, 50);
	flat2d::Entity* o2 = new EntityImpl(150, 150);
	flat2d::Entity* o3 = new EntityImpl(550, 550);
	flat2d::Entity* o4 = new EntityImpl(75, 75);
	flat2d::Entity* o5 = new EntityImpl(1095, 1095);

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

TEST_CASE( "Test spatial partitions with velocity", "[objectcontainer]" )
{
	flat2d::EntityContainer container;

	flat2d::Entity* o1 = new EntityImpl(50, 50);
	flat2d::Entity* o2 = new EntityImpl(150, 150);

	flat2d::EntityProperties& o1props = o1->getEntityProperties();
	o1props.setXvel(100);
	o1props.setYvel(100);

	flat2d::EntityProperties& o2props = o2->getEntityProperties();
	o2props.setXvel(-100);
	o2props.setYvel(-100);

	REQUIRE ( 0 == container.getSpatialPartitionCount() );

	container.registerObject(o1);

	REQUIRE ( 4 == container.getSpatialPartitionCount() );

	container.unregisterAllObjects();

	REQUIRE ( 0 == container.getSpatialPartitionCount() );

	container.registerObject(o2);

	REQUIRE ( 4 == container.getSpatialPartitionCount() );
}
