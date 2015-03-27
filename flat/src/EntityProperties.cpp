#include "EntityProperties.h"


namespace flat2d
{
	void EntityProperties::incrementXpos(int x)
	{
		this->x += x;
		setLocationChanged(true);
	}

	void EntityProperties::setXpos(int pos)
	{
		x = pos;
		setLocationChanged(true);
	}

	int EntityProperties::getXpos() const
	{
		return x;
	}

	void EntityProperties::incrementYpos(int y)
	{
		this->y += y;
		setLocationChanged(true);
	}

	void EntityProperties::setYpos(int pos)
	{
		y = pos;
		setLocationChanged(true);
	}

	int EntityProperties::getYpos() const
	{
		return y;
	}

	int EntityProperties::getWidth() const
	{
		return w;
	}

	int EntityProperties::getHeight() const
	{
		return h;
	}

	void EntityProperties::setXvel(float v)
	{
		xvel = v;
		setLocationChanged(true);
	}

	float EntityProperties::getXvel() const
	{
		return xvel;
	}

	void EntityProperties::setYvel(float v)
	{
		yvel = v;
		setLocationChanged(true);
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

	EntityShape EntityProperties::getVelocityColiderShape(float deltatime) const
	{
		float dx = xvel * deltatime;
		float dy = yvel * deltatime;

		EntityShape eShape = getColliderShape();
		EntityShape vShape;
		if (dx > 0) {
			vShape.x = eShape.x;
			vShape.w = eShape.w + dx;
		} else {
			vShape.x = eShape.x + dx;
			vShape.w = eShape.w - dx;
		}
		if (dy > 0) {
			vShape.y = eShape.y;
			vShape.h = eShape.h + dy;
		} else {
			vShape.y = eShape.y + dy;
			vShape.h = eShape.h - dy;
		}
		return vShape;
	}

	void EntityProperties::setColliderShape(EntityShape shape)
	{
		this->colliderShape = shape;
	}

	bool EntityProperties::containsPoint(int px, int py) const
	{
		return px >= x
			&& px <= x + w
			&& py >= y
			&& py <= y + h;
	}

	void EntityProperties::setLocationChanged(bool changed)
	{
		locationChanged = changed;
	}

	bool EntityProperties::hasLocationChanged() const
	{
		return locationChanged;
	}

	EntityProperties::Areas& EntityProperties::getCurrentAreas()
	{
		return currentAreas;
	}

	const EntityProperties::Areas& EntityProperties::getCurrentAreas() const
	{
		return currentAreas;
	}

	void EntityProperties::move(float deltatime)
	{
		int dx = xvel * deltatime;
		int dy = yvel * deltatime;

		if (dy != 0 || dx != 0) {
			setLocationChanged(true);
		}

		x += dx;
		y += dy;
	}
} // namespace flat2d
