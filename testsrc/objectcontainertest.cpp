#include "objectcontainertest.h"

#include "../src/objectcontainer.h"
#include "../src/character.h"

CPPUNIT_TEST_SUITE_REGISTRATION( ObjectContainerTest );

void ObjectContainerTest::test_container()
{
	Character* c = new Character(200, 200);
	ObjectContainer& container = ObjectContainer::getInstance();

	container.registerRenderable(c);
	container.registerRenderable(c);
	container.unregisterRenderable(c);

	container.registerEventHandler(c);
	container.registerEventHandler(c);
	container.unregisterEventHandler(c);
}
