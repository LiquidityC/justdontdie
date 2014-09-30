#include "RenderedGameObject.h"
#include "Camera.h"

using namespace flat2d;

void RenderedGameObject::init(SDL_Renderer *renderer, Camera *camera)
{
	this->camera = camera;
}

void RenderedGameObject::setDead(bool dead)
{
	this->dead = dead;
}

void RenderedGameObject::setClip(SDL_Rect& rect)
{
	clip = rect;
}

bool RenderedGameObject::isDead() const
{
	return dead;
}

void RenderedGameObject::setCamera(Camera *camera)
{
	this->camera = camera;
}

void RenderedGameObject::setCollidable(bool collidable)
{
	this->collidable = collidable;
}

void RenderedGameObject::render(SDL_Renderer* renderer) const
{
	if (texture == NULL) {
		return;
	}

	int x = xpos;
	int y = ypos;

	if (camera != NULL) {
		x = camera->getScreenXposFor(x);
		y = camera->getScreenYposFor(y);
	}

	SDL_Rect box = { x, y, width, height };
	SDL_RenderCopy(renderer, texture, &clip, &box);

	if (isCollider()) {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF );
		SDL_RenderDrawRect( renderer, &box );
	}
}

bool RenderedGameObject::isCollider() const
{
	return collidable;
}

SDL_Rect RenderedGameObject::getBoundingBox() const
{
	SDL_Rect box = { xpos, ypos, width, height };
	return box;
}

const SDL_Texture* RenderedGameObject::getTexture() const
{
	return texture;
}

void RenderedGameObject::setTexture(SDL_Texture* texture)
{
	this->texture = texture;
}
