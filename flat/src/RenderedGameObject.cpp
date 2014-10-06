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
	if (texture == NULL || dead) {
		return;
	}

	int x = xpos;
	int y = ypos;

	if (data->getCamera() != NULL) {
		Camera* camera = data->getCamera();
		SDL_Rect box = getBoundingBox();
		if (!camera->isVisibleOnCamera(box)) {
			return;
		}
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

void RenderedGameObject::setColliderBox(SDL_Rect collider)
{
	this->collider = collider;
}

SDL_Rect RenderedGameObject::getBoundingBox() const
{
	SDL_Rect box = { xpos, ypos, width, height };
	if (collider.w != 0 && collider.h != 0) {
		box.x += collider.x;
		box.y += collider.y;
		box.w = collider.w;
		box.h = collider.h;
	}
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
