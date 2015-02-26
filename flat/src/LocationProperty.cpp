#include "LocationProperty.h"

using namespace flat2d;

SDL_Rect LocationProperty::getBoundingBox() const
{
	return { xpos, ypos, width, height };
}
