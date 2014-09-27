#include <CppUTest/TestHarness.h>
#include "GameObjectImpl.h"
#include "../src/ObjectContainer.h"

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
	
	CHECK_EQUAL( 0, container.getObjectCount(0) );

	container.addLayer(0);
	container.addLayer(1);

	container.registerObject(c1);

	CHECK_EQUAL( 1, container.getObjectCount() );
	CHECK_EQUAL( 1, container.getObjectCount(flat2d::ObjectContainer::DEFAULT_LAYER) );

	container.registerObject(c2, 0);
	container.registerObject(c2, 1);

	CHECK_EQUAL( 2, container.getObjectCount() );
	CHECK_EQUAL( 1, container.getObjectCount(0) );
	CHECK_EQUAL( 0, container.getObjectCount(1) );

	container.registerObject(c3, 1);

	CHECK_EQUAL( 1, container.getObjectCount(1) );
	CHECK_EQUAL( 3, container.getObjectCount() );

	container.unregisterObject(c2);

	CHECK_EQUAL( 0, container.getObjectCount(0) );
	CHECK_EQUAL( 2, container.getObjectCount() );

	delete c2;
}
