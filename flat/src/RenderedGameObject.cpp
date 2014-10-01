#include "RenderedGameObject.h"
#include "Camera.h"
#include "RenderData.h"

using namespace flat2d;

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

void RenderedGameObject::setCollidable(bool collidable)
{
	this->collidable = collidable;
}

void RenderedGameObject::render(const RenderData *data) const
{
	if (texture == NULL) {
		return;
	}

	int x = xpos;
	int y = ypos;

	if (data->getCamera() != NULL) {
		Camera* camera = data->getCamera();
		x = camera->getScreenXposFor(x);
		y = camera->getScreenYposFor(y);
	}

	SDL_Rect box = { x, y, width, height };
	SDL_RenderCopy(data->getRenderer(), texture, &clip, &box);

#ifdef DEBUG
	if (isCollider()) {
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF );
		SDL_RenderDrawRect( renderer, &box );
	}
#endif
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
