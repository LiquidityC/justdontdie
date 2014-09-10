#include <CppUTest/TestHarness.h>
#include "../src/objectcontainer.h"
#include "../src/character.h"

TEST_GROUP( ObjectContainerTests )
{
};

TEST( ObjectContainerTests, TestObjectRegister )
{
	Character* c = new Character(100, 100);
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
	Character* c = new Character(100, 100);
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
	Character* c = new Character(100, 100);
	ObjectContainer container;

	container.unregisterObject(c);

	delete c;
}
