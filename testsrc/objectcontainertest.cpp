#include "objectcontainertest.h"

#include "../src/objectcontainer.h"
#include "../src/character.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ObjectContainerTest );

void ObjectContainerTest::test_container()
{
	Character* c = new Character(200, 200);
	ObjectContainer& container = ObjectContainer::getInstance();

	CPPUNIT_ASSERT( container.getRenderableCount() == 0 );
	container.registerRenderable(c);
	CPPUNIT_ASSERT( container.getRenderableCount() == 1 );
	container.registerRenderable(c);
	CPPUNIT_ASSERT( container.getRenderableCount() == 1 );
	container.unregisterRenderable(c);
	CPPUNIT_ASSERT( container.getRenderableCount() == 0 );

	CPPUNIT_ASSERT( container.getEventHandlerCount() == 0 );
	container.registerEventHandler(c);
	CPPUNIT_ASSERT( container.getEventHandlerCount() == 1 );
	container.registerEventHandler(c);
	CPPUNIT_ASSERT( container.getEventHandlerCount() == 1 );
	container.unregisterEventHandler(c);
	CPPUNIT_ASSERT( container.getEventHandlerCount() == 0 );
}
