#include <CppUTest/TestHarness.h>
#include "GameObjectImpl.h"
#include "../src/ObjectContainer.h"
#include "../src/GameData.h"
#include "../src/CollisionDetector.h"
#include "../src/Mixer.h"

TEST_GROUP( ObjectContainerTests )
{
};

TEST( ObjectContainerTests, TestObjectRegister )
{
	flat2d::GameObject* c = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer container;

	CHECK_EQUAL( 0, container.getObjectCount() );
	container.registerObject(c);
	CHECK_EQUAL( 1, container.getObjectCount() );
	container.registerObject(c);
	CHECK_EQUAL( 1, container.getObjectCount() );
	container.unregisterObject(c);
	CHECK_EQUAL( 0, container.getObjectCount() );

	delete c;
}

TEST( ObjectContainerTests, TestObjectContainerDestructor )
{
	flat2d::GameObject* c = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer* container = new flat2d::ObjectContainer();

	CHECK_EQUAL( 0, container->getObjectCount() );

	container->registerObject(c);

	CHECK_EQUAL( 1, container->getObjectCount() );

	container->registerObject(c);

	CHECK_EQUAL( 1, container->getObjectCount() );

	delete container;
}

TEST( ObjectContainerTests, TestObjectContainerUnregister )
{
	flat2d::GameObject* c = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer container;

	container.unregisterObject(c);

	delete c;
}

TEST( ObjectContainerTests, TestObjectContainerLayers )
{
	flat2d::GameObject* c1 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c2 = new GameObjectImpl(100, 100);
	flat2d::GameObject* c3 = new GameObjectImpl(100, 100);
	flat2d::ObjectContainer container;
	
	CHECK_EQUAL( 0, container.getObjectCountFor(0) );

	container.addLayer(0);
	container.addLayer(0);
	container.addLayer(1);

	container.registerObject(c1);

	CHECK_EQUAL( 1, container.getObjectCount() );
	CHECK_EQUAL( 1, container.getObjectCountFor(flat2d::ObjectContainer::DEFAULT_LAYER) );

	container.registerObject(c2, 0);
	container.registerObject(c2, 1);

	CHECK_EQUAL( 2, container.getObjectCount() );
	CHECK_EQUAL( 1, container.getObjectCountFor(0) );
	CHECK_EQUAL( 0, container.getObjectCountFor(1) );

	container.registerObject(c3, 1);

	CHECK_EQUAL( 1, container.getObjectCountFor(1) );
	CHECK_EQUAL( 3, container.getObjectCount() );

	container.unregisterObject(c2);

	CHECK_EQUAL( 0, container.getObjectCountFor(0) );
	CHECK_EQUAL( 2, container.getObjectCount() );

	delete c2;
}

TEST( ObjectContainerTests, TestObjectContainerClearLayers )
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

	CHECK_EQUAL ( 6, container.getObjectCount() );
	CHECK_EQUAL ( 3, container.getObjectCountFor(frontLayer) );
	CHECK_EQUAL ( 3, container.getObjectCountFor(backLayer) );

	container.unregisterAllObjectsFor(frontLayer);

	CHECK_EQUAL ( 3, container.getObjectCount() );
	CHECK_EQUAL ( 0, container.getObjectCountFor(frontLayer) );
	CHECK_EQUAL ( 3, container.getObjectCountFor(backLayer) );
}

TEST( ObjectContainerTests, TestObjectAutoclean )
{
	flat2d::GameObject* c1 = new GameObjectImpl(100, 100);
	GameObjectImpl* c2 = new GameObjectImpl(100, 100);

	flat2d::ObjectContainer container;
	flat2d::CollisionDetector detector(&container);
	flat2d::Mixer mixer;
	flat2d::GameData gameData(&container, &detector, &mixer);

	container.registerObject(c1);
	container.registerObject(c2);

	CHECK_EQUAL ( 2, container.getObjectCount() );

	c2->setDead(true);
	container.preHandleObjects(&gameData);

	CHECK_EQUAL ( 1, container.getObjectCount() );
}
