#include <CppUTest/TestHarness.h>
#include "../src/objectcontainer.h"
#include "../src/character.h"

TEST_GROUP( ObjectContainerTests )
{
};

TEST( ObjectContainerTests, TestRenderableRegister )
{
	Character* c = new Character(100, 100);

	ObjectContainer container;

	CHECK_EQUAL( 0, container.getRenderableCount() );
	container.registerRenderable(c);
	CHECK_EQUAL( 1, container.getRenderableCount() );
	container.registerRenderable(c);
	CHECK_EQUAL( 1, container.getRenderableCount() );
	container.unregisterRenderable(c);
	CHECK_EQUAL( 0, container.getRenderableCount() );

	delete c;
}

TEST( ObjectContainerTests, TestEventHandlerRegister )
{
	Character* c = new Character(100, 100);
	ObjectContainer container;

	CHECK_EQUAL( 0, container.getEventHandlerCount() );
	container.registerEventHandler(c);
	CHECK_EQUAL( 1, container.getEventHandlerCount() );
	container.registerEventHandler(c);
	CHECK_EQUAL( 1, container.getEventHandlerCount() );
	container.unregisterEventHandler(c);
	CHECK_EQUAL( 0, container.getEventHandlerCount() );

	delete c;
}

TEST( ObjectContainerTests, TestObjectContainerObjects )
{
	Character* c = new Character(100, 100);
	ObjectContainer container;

	CHECK_EQUAL( 0, container.getEventHandlerCount() );
	CHECK_EQUAL( 0, container.getRenderableCount() );
	CHECK_EQUAL( 0, container.getObjectCount() );

	container.registerRenderable(c);

	CHECK_EQUAL( 0, container.getEventHandlerCount() );
	CHECK_EQUAL( 1, container.getRenderableCount() );
	CHECK_EQUAL( 1, container.getObjectCount() );

	container.registerEventHandler(c);

	CHECK_EQUAL( 1, container.getEventHandlerCount() );
	CHECK_EQUAL( 1, container.getRenderableCount() );
	CHECK_EQUAL( 1, container.getObjectCount() );
}

IGNORE_TEST( ObjectContainerTests, TestObjectContainerDestructor )
{
	Character* c = new Character(100, 100);
	ObjectContainer* container = new ObjectContainer();

	container->registerEventHandler(c);
	container->registerRenderable(c);

	delete container;
}
