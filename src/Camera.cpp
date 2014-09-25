#include "Camera.h"

int Camera::getXpos()
{
	return xpos;
}

int Camera::getYpos()
{
	return ypos;
}

int Camera::getWidth()
{
	return width;
}

int Camera::getHeight()
{
	return height;
}

SDL_Rect Camera::getBox()
{
	SDL_Rect box = { xpos, ypos, width, height };
	return box;
}

void Camera::centerOn(int x, int y) {
	xpos = x - (width/2);
	ypos = y - (height/2);

	if (xpos > MAP_WIDTH - width) {
		xpos = MAP_WIDTH - width;
	} else if (xpos < 0) {
		xpos = 0;
	}
	if (ypos > MAP_HEIGHT - height) {
		ypos = MAP_HEIGHT - height;
	} else if (ypos < 0) {
		ypos = 0;
	}
}

bool Camera::isVisibleOnCamera(SDL_Rect& box)
{
	if (box.x > xpos + width) {
		return false;
	} else if (box.x + box.w < xpos) {
		return false;
	} else if (box.y > ypos + height) {
		return false;
	} else if (box.y + box.h < ypos) {
		return false;
	}
	return true;
}

int Camera::getScreenXposFor(int x)
{
	return x - xpos;
}

int Camera::getScreenYposFor(int y)
{
	return y - ypos;
}
