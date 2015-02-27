#include "LocationProperty.h"

using namespace flat2d;

SDL_Rect LocationProperty::getBoundingBox() const
{
	return { xpos, ypos, width, height };
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
