#include <iostream>
#include "LocationProperty.h"

using namespace flat2d;

void LocationProperty::setXpos(int x)
{
	xpos = x;
}

void LocationProperty::incrementXpos(int x)
{
	xpos += x;
	locationUpdated();
}

int LocationProperty::getXpos() const
{
	return xpos;
}

void LocationProperty::setYpos(int y)
{
	ypos = y;
	locationUpdated();
}

void LocationProperty::incrementYpos(int y)
{
	ypos += y;
	locationUpdated();
}

int LocationProperty::getYpos() const
{
	return ypos;
}

void LocationProperty::setWidth(int w) {
	width = w;
	locationUpdated();
}

int LocationProperty::getWidth() const
{
	return width;
}

void LocationProperty::setHeight(int h)
{
	height = h;
	locationUpdated();
}

int LocationProperty::getHeight() const
{
	return height;
}

SDL_Rect LocationProperty::getBoundingBox() const
{
	return { xpos, ypos, width, height };
}

void LocationProperty::setOnLocationChange(OnLocationChangeFunction onChange)
{
	this->onLocationChange = onChange;
}

bool LocationProperty::containsPoint(int x, int y) const
{
	return x >= xpos
		&& x <= xpos + width
		&& y >= ypos 
		&& y <= ypos + height;
}

void LocationProperty::locationUpdated()
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

	if (locationChanged && onLocationChange) {
		try {
			std::cout << "Location change: " << parents.size() << std::endl;
			onLocationChange();
			std::cout << "Lambda called" << std::endl;
		} catch (const std::exception& e) {
			std::cout << "Handled exception" << std::endl;
			std::cout << e.what();
		} catch (...) {
			std::cout << "Unhandled exception" << std::endl;
		}
	}
}

LocationProperty::Parents& LocationProperty::getParents()
{
	return parents;
}

bool LocationProperty::operator<(const LocationProperty& loc) const
{
	if (xpos == loc.xpos) {
		return ypos < loc.ypos;
	} else {
		return xpos < loc.xpos;
	}
}

bool LocationProperty::operator==(const LocationProperty& loc) const
{
	return width == loc.width && height == loc.height && !(*this < loc) && !(loc < *this);
}

bool LocationProperty::operator!=(const LocationProperty& loc) const
{
	return !(*this == loc);
}
