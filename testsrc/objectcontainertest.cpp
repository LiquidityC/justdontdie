#include <CppUTest/TestHarness.h>
#include "../src/objectcontainer.h"
#include "../src/Block.h"

TEST_GROUP( ObjectContainerTests )
{
};

TEST( ObjectContainerTests, TestObjectRegister )
{
	Block* c = new Block(100, 100);
	ObjectContainer container;

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
	Block* c = new Block(100, 100);
	ObjectContainer* container = new ObjectContainer();

	CHECK_EQUAL( 0, container->getObjectCount() );

	container->registerObject(c);

	CHECK_EQUAL( 1, container->getObjectCount() );

	container->registerObject(c);

	CHECK_EQUAL( 1, container->getObjectCount() );

	delete container;
}

TEST( ObjectContainerTests, TestObjectContainerUnregister )
{
	Block* c = new Block(100, 100);
	ObjectContainer container;

	container.unregisterObject(c);

	delete c;
}

TEST (ObjectContainerTests, TestSelfCleaning ) 
{
	ObjectContainer container;

	// Should not be cleared
	container.registerObject(new Block(-5, -5));
	container.registerObject(new Block(95, 95));
	container.registerObject(new Block(50, 50));

	// Chould be cleared
	container.registerObject(new Block(101, 101));
	container.registerObject(new Block(-100, -100));
	container.registerObject(new Block(105, 105));

	CHECK_EQUAL( 6, container.getObjectCount() );

	container.cleanNonVisibleObjects(100, 100);

	CHECK_EQUAL( 3, container.getObjectCount() );
}
