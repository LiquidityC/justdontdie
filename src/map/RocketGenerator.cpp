#include "RocketGenerator.h"
#include "MapTileObject.h"
#include "../npcs/Rocket.h"
#include "../LayerService.h"
#include "../CustomGameData.h"

void RocketGenerator::preMove(const flat2d::GameData *gameData, const MapTileObject *tile)
{
	if (launchTimer.isStarted() && launchTimer.getTicks() < 2000) {
		return;
	}

	launchTimer.start();

	Rocket::Mode mode = Rocket::Mode::NORMAL;
	if (tile->hasProperty("ghost")) {
		mode = Rocket::Mode::GHOST;
	} else if (tile->hasProperty("multi")) {
		mode = Rocket::Mode::MULTI;
	}

	LayerService *layerService = static_cast<CustomGameData*>(gameData->getCustomGameData())->getLayerService();

	const flat2d::EntityProperties &tileProps = tile->getEntityProperties();
	Rocket *rocket = new Rocket(tileProps.getXpos(), tileProps.getYpos(), mode, !tile->hasProperty("shootRight"));
	rocket->init(gameData);
	gameData->getEntityContainer()->registerObject(rocket, layerService->getLayerIndex(FRONT_LAYER));
}
