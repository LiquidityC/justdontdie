#include <iostream>
#include "Entity.h"
#include "Camera.h"
#include "RenderData.h"

using namespace flat2d;

void Entity::setDead(bool dead)
{
	this->dead = dead;
}

void Entity::setClip(SDL_Rect& rect)
{
	clip = rect;
}

bool Entity::isDead() const
{
	return dead;
}

void Entity::setCollidable(bool collidable)
{
	this->collidable = collidable;
}

void Entity::render(const RenderData *data) const
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

bool Entity::isCollider() const
{
	return collidable;
}

void Entity::setColliderBox(SDL_Rect collider)
{
	this->collider = collider;
}

SDL_Rect Entity::getBoundingBox() const
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

const SDL_Texture* Entity::getTexture() const
{
	return texture;
}

void Entity::setTexture(SDL_Texture* texture)
{
	this->texture = texture;
}

bool Entity::isFixedPosition()
{
	return fixedPosition;
}

void Entity::setFixedPosition(bool fixedPosition)
{
	this->fixedPosition = fixedPosition;
}

LocationProperty& Entity::getLocationProperty()
{
	return locationProperty;
}

const LocationProperty& Entity::getLocationProperty() const
{
	return locationProperty;
}
