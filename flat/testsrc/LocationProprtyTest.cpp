#include <CppUTest/TestHarness.h>
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

