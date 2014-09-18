#include "character.h"

void Character::postHandle()
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

void Character::render(SDL_Surface& surface) const
{
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
	SDL_FillRect(&surface, &box, SDL_MapRGB(surface.format, 0xFF, 0xFF, 0xFF));
}

bool Character::isCollider()
{
	return true;
}

SDL_Rect Character::getBoundingBox()
{
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
	return box;
}
