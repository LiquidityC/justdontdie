#include "Ship.h"

void Ship::postHandle()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_UP]) {
		ypos -= 3;
	}
	if (currentKeyStates[SDL_SCANCODE_DOWN]) {
		ypos += 3;
	}
	if (currentKeyStates[SDL_SCANCODE_LEFT]) {
		xpos -= 3;
	}
	if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
		xpos += 3;
	}
}

void Ship::render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor( renderer, 0x00, 0xCC, 0x00, 0xFF );
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
	SDL_RenderFillRect(renderer, &box);
}

SDL_Rect Ship::getBoundingBox() const
{
	SDL_Rect rect = { xpos, ypos, WIDTH, HEIGHT };
	return rect;
}
