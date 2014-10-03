#include "Rocket.h"

void Rocket::preRender(const flat2d::RenderData *data)
{
	flat2d::Camera *cam = data->getCamera();
	SDL_Rect boundingBox = getBoundingBox();
	if (!cam->isVisibleOnCamera(boundingBox)) {
		setDead(true);
		return;
	}

	xpos += (xvel * cam->getDeltaTime());
}

bool Rocket::isGhost() const
{
	return ghostMode;
}
