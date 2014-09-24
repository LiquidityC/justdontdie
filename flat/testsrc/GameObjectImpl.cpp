#include "GameObjectImpl.h"

bool GameObjectImpl::isCollider() const
{
	return true;
}

SDL_Rect GameObjectImpl::getBoundingBox() const
{
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
	return box;
}
