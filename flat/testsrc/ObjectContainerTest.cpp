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
