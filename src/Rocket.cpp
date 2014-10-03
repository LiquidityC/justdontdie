#include "Rocket.h"

void Rocket::init(const flat2d::RenderData *data)
{
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/rocket.png", data->getRenderer()));
}

void Rocket::preRender(const flat2d::RenderData *data)
{
	flat2d::Camera *cam = data->getCamera();
	xpos += (xvel * cam->getDeltaTime());
}

bool Rocket::isGhost() const
{
	return ghostMode;
}
