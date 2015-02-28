#include <CppUTest/TestHarness.h>
#include <iostream>
#include "GameObjectImpl.h"
#include "../src/LocationProperty.h"

using namespace flat2d;

TEST_GROUP( LocationPropertyTests )
{
};

TEST( LocationPropertyTests, TestPointMatching )
{
	LocationProperty lop(100, 100, 100);

	CHECK( lop.containsPoint(150, 150) );
	CHECK( lop.containsPoint(105, 105) );
	CHECK( lop.containsPoint(100, 100) );
	CHECK( lop.containsPoint(200, 200) );

	CHECK( !lop.containsPoint(201, 201) );
	CHECK( !lop.containsPoint(50, 50) );
	CHECK( !lop.containsPoint(99, 99) );
}

TEST( LocationPropertyTests, TestEquality )
{
	LocationProperty lop1(10, 20, 40);
	LocationProperty lop2(10, 20, 40);
	LocationProperty lop3(10, 10, 40);

	CHECK( lop1 == lop2 );
	CHECK( !(lop1 == lop3) );
	CHECK( lop1 != lop3 );
	CHECK( !(lop1 != lop2) );
}

TEST( LocationPropertyTests, TestLessOperator )
{
	LocationProperty lop1(10, 20, 40);
	LocationProperty lop2(10, 20, 40);
	LocationProperty lop3(10, 10, 40);
	LocationProperty lop4(5, 30, 40);

	CHECK( !(lop1 < lop2) );
	CHECK( !(lop2 < lop1) );
	CHECK( lop3 < lop1 );
	CHECK( lop4 < lop1 );
	CHECK( !(lop1 < lop3) );
	CHECK( !(lop1 < lop4) );
}

TEST( LocationPropertyTests, TestChangeTrigger )
{
	bool hasMovedOutOfSpace = false;
	GameObjectImpl o(100, 100);

	LocationProperty space(0, 0, 200, 200);
	LocationProperty& lop = o.getLocationProperty();
	lop.getParents().push_back(space);

	CHECK_EQUAL( 1, lop.getParents().size() );

	lop.setOnLocationChange([&]() { 
			hasMovedOutOfSpace = true;
			});

	lop.setXpos(150);
	CHECK( !hasMovedOutOfSpace );

	lop.setXpos(250);
	CHECK( hasMovedOutOfSpace );
	hasMovedOutOfSpace = false;

	lop.setYpos(250);
	CHECK( hasMovedOutOfSpace );
	hasMovedOutOfSpace = false;

	lop.incrementXpos(250);
	CHECK( hasMovedOutOfSpace );
	hasMovedOutOfSpace = false;

	lop.incrementYpos(250);
	CHECK( hasMovedOutOfSpace );
	hasMovedOutOfSpace = false;
}
