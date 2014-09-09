#ifndef _GENERIC_FIXTURE_H
#define _GENERIC_FIXTURE_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class GenericFixture : public CppUnit::TestFixture
{
	public:
		virtual void setUp() {  };
		virtual void tearDown() {  };
};

#endif
