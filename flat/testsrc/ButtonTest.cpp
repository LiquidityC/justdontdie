#include <CppUTest/TestHarness.h>
#include "GameObjectImpl.h"
#include "../src/Button.h"

TEST_GROUP ( ButtonTests )
{

};

TEST (ButtonTests, TestClick)
{
	bool clicked = false;
	flat2d::ui::Button button(10, 10, 30, 10, 
			[&]() {
				clicked = true;
			});

	CHECK( !clicked );

	button.triggerClick();

	CHECK( clicked );
}
