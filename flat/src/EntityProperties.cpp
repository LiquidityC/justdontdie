#include <iostream>
#include "EntityProperties.h"

using namespace flat2d;

void EntityProperties::setXpos(int x)
{
	xpos = x;
	locationUpdated();
}

void EntityProperties::incrementXpos(int x)
{
	xpos += x;
	locationUpdated();
}

int EntityProperties::getXpos() const
{
	return xpos;
}

void EntityProperties::setYpos(int y)
{
	ypos = y;
	locationUpdated();
}

void EntityProperties::incrementYpos(int y)
{
	ypos += y;
	locationUpdated();
}

int EntityProperties::getYpos() const
{
	return ypos;
}

void EntityProperties::setWidth(int w) {
	width = w;
	locationUpdated();
}

int EntityProperties::getWidth() const
{
	return width;
}

void EntityProperties::setHeight(int h)
{
	height = h;
	locationUpdated();
}

int EntityProperties::getHeight() const
{
	return height;
}

SDL_Rect EntityProperties::getBoundingBox() const
{
	return { xpos, ypos, width, height };
}

void EntityProperties::setOnLocationChange(OnLocationChangeFunction onChange)
{
	if (!onLocationChange) {
		onLocationChange = onChange;
	}
}

bool EntityProperties::containsPoint(int x, int y) const
{
	return x >= xpos
		&& x <= xpos + width
		&& y >= ypos 
		&& y <= ypos + height;
}

void EntityProperties::locationUpdated()
{
	if (!onLocationChange || parents.empty()) {
		return;
	}

	bool locationChanged = false;
	for (auto it = parents.begin(); it != parents.end(); ++it) {
		if (!it->containsPoint(xpos, ypos)
				|| !it->containsPoint(xpos + width, ypos)
				|| !it->containsPoint(xpos, ypos + height)
				|| !it->containsPoint(xpos + width, ypos + height))
		{
			locationChanged = true;
			break;
		}
	}

	if (locationChanged) {
		onLocationChange();
	}
}

EntityProperties::Parents& EntityProperties::getParents()
{
	return parents;
}

const EntityProperties::Parents& EntityProperties::getParents() const
{
	return parents;
}

bool EntityProperties::operator<(const EntityProperties& loc) const
{
	if (xpos == loc.xpos) {
		return ypos < loc.ypos;
	} else {
		return xpos < loc.xpos;
	}
}

bool EntityProperties::operator==(const EntityProperties& loc) const
{
	return width == loc.width && height == loc.height && !(*this < loc) && !(loc < *this);
}

bool EntityProperties::operator!=(const EntityProperties& loc) const
{
	return !(*this == loc);
}
