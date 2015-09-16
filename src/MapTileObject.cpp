#include <string>

#include "MapTileObject.h"
#include "Rocket.h"
#include "CustomGameData.h"
#include "LayerService.h"
#include "Random.h"

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
	return true;
}

void MapTileObject::preMove(const flat2d::GameData *gameData)
{
	if (hasProperty("rocketLauncher") && (!launchTimer.isStarted() || launchTimer.getTicks() > 2000)) {
		launchTimer.start();

		Rocket::Mode mode = Rocket::Mode::NORMAL;
		if (hasProperty("ghost")) {
			mode = Rocket::Mode::GHOST;
		} else if (hasProperty("multi")) {
			mode = Rocket::Mode::MULTI;
		}

		LayerService *layerService = static_cast<CustomGameData*>(gameData->getCustomGameData())->getLayerService();

		Rocket *rocket = new Rocket(entityProperties.getXpos(), entityProperties.getYpos(), mode, !hasProperty("shootRight"));
		rocket->init(gameData);
		gameData->getEntityContainer()->registerObject(rocket, layerService->getLayerIndex(FRONT_LAYER));
	}

	if (hidden && hiddenTimer.isStarted() && hiddenTimer.getTicks() > 5000) {
		hiddenTimer.stop();
		hidden = false;
	}

	tileBreakEmitter->emit(gameData, entityProperties.getBoundingBox());
}

bool MapTileObject::isHidden() const
{
	return hidden;
}

void MapTileObject::hide()
{
	hidden = true;
	hiddenTimer.start();
}

void MapTileObject::destroy(const flat2d::GameData *data)
{
	setDead(true);
	tileBreakEmitter->setEmissionCount(200);
}

void MapTileObject::render(const flat2d::RenderData *data) const
{
	if (hidden) {
		return;
	}

	Entity::render(data);
#ifdef DEBUG
	if (texture == nullptr && !dead) {
		SDL_SetRenderDrawColor(data->getRenderer(), 0x00, 0x00, 0xFF, 0xFF );
		SDL_Rect boundingBox = entityProperties.getBoundingBox();
		boundingBox.x = data->getCamera()->getScreenXposFor(boundingBox.x);
		boundingBox.y = data->getCamera()->getScreenYposFor(boundingBox.y);
		SDL_RenderDrawRect( data->getRenderer(), &boundingBox);
	}
#endif
}
