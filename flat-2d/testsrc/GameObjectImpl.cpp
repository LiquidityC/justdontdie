#include "GameObjectImpl.h"

void GameObjectImpl::preHandle()
{
	ypos -= 5;
}

void GameObjectImpl::render(SDL_Surface& surface) const
{
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
	SDL_FillRect(&surface, &box, SDL_MapRGB(surface.format, 0xFF, 0xFF, 0xFF));
}

bool GameObjectImpl::isCollider() const
{
	return true;
}

SDL_Rect GameObjectImpl::getBoundingBox() const
{
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
	return box;
}
