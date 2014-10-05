#include "Rocket.h"

void Rocket::init(const flat2d::RenderData *data)
{
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/rocket.png", data->getRenderer()));
	deathTimer.start();
}

void Rocket::preRender(const flat2d::RenderData *data)
{
	flat2d::Camera *cam = data->getCamera();
	xpos += (xvel * cam->getDeltaTime());
}

void Rocket::postRender(const flat2d::RenderData *data)
{
	setDead(deathTimer.getTicks() > 5000);
}

bool Rocket::isGhost() const
{
	return ghostMode;
}
