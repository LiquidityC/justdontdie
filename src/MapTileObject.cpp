#include "MapTileObject.h"

void MapTileObject::setCollidable(bool collidable)
{
	properties["collidable"] = collidable;
}

bool MapTileObject::isCollider() const
{
	return hasProperty("collidable");
}

void MapTileObject::setProperty(std::string prop, bool value)
{
	properties[prop] = value;
}

bool MapTileObject::hasProperty(std::string prop) const
{
	auto pair = properties.find(prop);
	if (pair == properties.end()) {
		return false;
	}
	return pair->second;
}
