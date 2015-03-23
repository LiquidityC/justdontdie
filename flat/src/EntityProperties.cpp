#include "EntityProperties.h"


namespace flat2d
{
	void EntityProperties::setXpos(int x)
	{
		this->x = x;
		locationUpdated();
	}

	void EntityProperties::incrementXpos(int x)
	{
		this->x += x;
		locationUpdated();
	}

	int EntityProperties::getXpos() const
	{
		return x;
	}

	void EntityProperties::setYpos(int y)
	{
		this->y = y;
		locationUpdated();
	}

	void EntityProperties::incrementYpos(int y)
	{
		this->y += y;
		locationUpdated();
	}

	int EntityProperties::getYpos() const
	{
		return y;
	}

	void EntityProperties::setWidth(int w) {
		this->w = w;
		locationUpdated();
	}

	int EntityProperties::getWidth() const
	{
		return w;
	}

	void EntityProperties::setHeight(int h)
	{
		this->h = h;
		locationUpdated();
	}

	int EntityProperties::getHeight() const
	{
		return h;
	}

	void EntityProperties::setXvel(float v)
	{
		xvel = v;
	}

	float EntityProperties::getXvel() const
	{
		return xvel;
	}

	void EntityProperties::setYvel(float v)
	{
		yvel = v;
	}

	float EntityProperties::getYvel() const
	{
		return yvel;
	}

	bool EntityProperties::isMoving() const
	{
		return xvel != 0 || yvel != 0;
	}

	SDL_Rect EntityProperties::getBoundingBox() const
	{
		return { x, y, w, h };
	}

	void EntityProperties::setCollidable(bool collidable)
	{
		this->collidable = collidable;
	}

	bool EntityProperties::isCollidable() const
	{
		return collidable;
	}

	EntityShape EntityProperties::getColliderShape() const
	{
		return {
			x + colliderShape.x,
			y + colliderShape.y,
			colliderShape.w,
			colliderShape.h
		};
	}

	void EntityProperties::setColliderShape(EntityShape shape)
	{
		this->colliderShape = shape;
	}

	void EntityProperties::setOnLocationChange(OnLocationChangeFunction onChange)
	{
		if (!onLocationChange) {
			onLocationChange = onChange;
		}
	}

	bool EntityProperties::containsPoint(int px, int py) const
	{
		return px >= x
			&& px <= x + w
			&& py >= y
			&& py <= y + h;
	}

	void EntityProperties::locationUpdated()
	{
		if (!onLocationChange || currentAreas.empty()) {
			return;
		}

		bool locationChanged = false;
		for (auto it = currentAreas.begin(); it != currentAreas.end(); ++it) {
			if (!it->containsPoint(x, y)
					|| !it->containsPoint(x + w, y)
					|| !it->containsPoint(x, y + h)
					|| !it->containsPoint(x + w, y + h))
			{
				locationChanged = true;
				break;
			}
		}

		if (locationChanged) {
			onLocationChange();
		}
	}

	EntityProperties::Areas& EntityProperties::getCurrentAreas()
	{
		return currentAreas;
	}

	const EntityProperties::Areas& EntityProperties::getCurrentAreas() const
	{
		return currentAreas;
	}
} // namespace flat2d
