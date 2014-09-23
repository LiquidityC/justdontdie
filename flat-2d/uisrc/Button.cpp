#include "Button.h"

using namespace flat2d::ui;

void Button::handle(const SDL_Event& e)
{
	if (e.type != SDL_MOUSEMOTION) {
		return;
	}

	int x = e.motion.x;
	int y = e.motion.y;

	if (x < xpos || x > static_cast<int>(xpos + width)) {
		return;
	} else if (y < ypos || y > static_cast<int>(ypos + height)) {
		return;
	}

	triggerClick();
}

void Button::render(SDL_Renderer* renderer) const
{

}

void Button::setOnClick(std::function<void ()> click) 
{
	onClick = click;
}

void Button::triggerClick() 
{
	onClick();
}
