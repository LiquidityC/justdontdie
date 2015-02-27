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
	if (texture == nullptr || dead) {
		return;
	}

	SDL_Rect box = locationProperty.getBoundingBox();
	if (data->getCamera() != nullptr && !fixedPosition) {
		Camera* camera = data->getCamera();
		if (!camera->isVisibleOnCamera(box)) {
			return;
		}
		box.x = camera->getScreenXposFor(box.x);
		box.y = camera->getScreenYposFor(box.y);
	}

	SDL_RenderCopy(data->getRenderer(), texture, &clip, &box);

#ifdef DEBUG
	if (isCollider()) {
		SDL_SetRenderDrawColor(data->getRenderer(), 0xFF, 0x00, 0x00, 0xFF );
		SDL_Rect bounds = getBoundingBox();
		if (data->getCamera() != nullptr) {
			bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
			bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
		}
		SDL_RenderDrawRect( data->getRenderer(), &bounds );
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
	SDL_Rect box = locationProperty.getBoundingBox();
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

bool RenderedGameObject::isFixedPosition()
{
	return fixedPosition;
}

void RenderedGameObject::setFixedPosition(bool fixedPosition)
{
	this->fixedPosition = fixedPosition;
}

const LocationProperty& RenderedGameObject::getLocationProperty()
{
	return locationProperty;
}
