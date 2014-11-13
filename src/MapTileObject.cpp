#include "MapTileObject.h"
#include "Rocket.h"
#include "LayerService.h"

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

void MapTileObject::preRender(const flat2d::RenderData *renderData)
{
	if (hasProperty("rocketLauncher") && (!launchTimer.isStarted() || launchTimer.getTicks() > 2000)) {
		launchTimer.start();

		Rocket::Mode mode = Rocket::Mode::NORMAL;
		if (hasProperty("ghost")) {
			mode = Rocket::Mode::GHOST;
		} else if (hasProperty("multi")) {
			mode = Rocket::Mode::MULTI;
		}

		Rocket *rocket = new Rocket(xpos, ypos, mode, !hasProperty("shootRight"));
		rocket->init(renderData);
		renderData->getObjectContainer()->registerObject(rocket, Layers::FRONT);
	}
}
