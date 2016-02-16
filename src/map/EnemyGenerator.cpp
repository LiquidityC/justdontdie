#include <string>
#include "EnemyGenerator.h"
#include "MapTileObject.h"
#include "../LayerService.h"
#include "../CustomGameData.h"
#include "../npcs/AngryBot.h"

void EnemyGenerator::preMove(const flat2d::GameData *gameData, const MapTileObject *tile)
{
	if (!timeToSpawn() || !tile->hasProperty("spawn")) {
		return;
	}

	const flat2d::EntityProperties& props = tile->getEntityProperties();
	std::string spawn = tile->getProperty("spawn");
	Enemy *enemy = nullptr;
	if (spawn == "angryBot") {
		enemy = new AngryBot(props.getXpos(), props.getYpos());
		enemy->init(gameData);
	}

	if (enemy) {
		LayerService *layerService = static_cast<CustomGameData*>(gameData->getCustomGameData())->getLayerService();
		gameData->getEntityContainer()->registerObject(enemy, layerService->getLayerIndex(FRONT_LAYER));
	}
}
