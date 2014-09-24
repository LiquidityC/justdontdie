#include "Button.h"

using namespace flat2d::ui;

void Button::handle(const SDL_Event& e)
{
	if (e.type == SDL_MOUSEMOTION) {
		int x = e.motion.x;
		int y = e.motion.y;

		if (x < xpos || x > xpos + width) {
			mouseIsOver = false;
			return;
		} 
		if (y < ypos || y > ypos + height) {
			mouseIsOver = false;
			return;
		}
		mouseIsOver = true;
	}

	if (e.type == SDL_MOUSEBUTTONDOWN 
			&& e.button.button == SDL_BUTTON_LEFT
			&& mouseIsOver) 
	{
		triggerClick();
	}
}

void Button::render(SDL_Renderer* renderer) const
{
	// Draw the box
	SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 0xFF);
	SDL_Rect rect = { xpos, ypos, width, height };
	SDL_RenderFillRect(renderer, &rect);

	// Draw the border
	if (mouseIsOver) {
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	} else {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	SDL_RenderDrawRect(renderer, &rect);
}

void Button::setOnClick(std::function<void ()> click) 
{
	onClick = click;
}

void Button::triggerClick() 
{
	onClick();
}
