#ifndef _OBJECT_CONTAINER_TEST_H
#define _OBJECT_CONTAINER_TEST_H

#include "genericfixture.h"

class ObjectContainerTest : public GenericFixture
{
	public:

		CPPUNIT_TEST_SUITE( ObjectContainerTest );
		CPPUNIT_TEST( test_container );
		CPPUNIT_TEST_SUITE_END();

	public:

		void test_container();
};

#endif
