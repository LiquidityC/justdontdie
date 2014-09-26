#include <iostream>
#include "GenericGameObject.h"
#include "CompContainer.h"

void GenericGameObject::render(SDL_Renderer* renderer) const
{
	Camera& cam = CompContainer::getInstance().getCamera();

	SDL_Rect srcBox = { clipX, clipY, width, height };
	SDL_Rect destBox = { cam.getScreenXposFor(xpos), cam.getScreenYposFor(ypos), width, height };
	SDL_RenderCopy(renderer, texture, &srcBox, &destBox);
}

void GenericGameObject::setCollidable(bool collidable)
{
	collider = collidable;
}

bool GenericGameObject::isCollider() const
{
	return collider;
}

void GenericGameObject::setClipCoordinates(int x, int y)
{
	clipX = x;
	clipY = y;
}

SDL_Rect GenericGameObject::getBoundingBox() const
{
	SDL_Rect box = { xpos, ypos, width, height };
	return box;
}
