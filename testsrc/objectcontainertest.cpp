#include <CppUTest/TestHarness.h>
#include "../src/objectcontainer.h"
#include "../src/character.h"

TEST_GROUP( ObjectContainerTests )
{
	void setup()
	{
		ObjectContainer::getInstance();
	}

	void teardown()
	{
	}
};

TEST( ObjectContainerTests, TestRenderableRegister )
{
	Character* c = new Character(100, 100);

	ObjectContainer& container = ObjectContainer::getInstance();

	CHECK( container.getRenderableCount() == 0 );
	container.registerRenderable(c);
	CHECK( container.getRenderableCount() == 1 );
	container.registerRenderable(c);
	CHECK( container.getRenderableCount() == 1 );
	container.unregisterRenderable(c);
	CHECK( container.getRenderableCount() == 0 );

	delete c;
}

TEST( ObjectContainerTests, TestEventHandlerRegister )
{
	Character* c = new Character(100, 100);

	ObjectContainer& container = ObjectContainer::getInstance();

	CHECK( container.getEventHandlerCount() == 0 );
	container.registerEventHandler(c);
	CHECK( container.getEventHandlerCount() == 1 );
	container.registerEventHandler(c);
	CHECK( container.getEventHandlerCount() == 1 );
	container.unregisterEventHandler(c);
	CHECK( container.getEventHandlerCount() == 0 );

	delete c;
}
