#include "ProximityEnemyGenerator.h"
#include "MapTileObject.h"

bool ProximityEnemyGenerator::timeToSpawn()
{
	if (hasSpawned) {
		return false;
	}

	if (!spawnTime) {
		return false;
	}

	hasSpawned = true;
	return true;
}

void ProximityEnemyGenerator::preMove(const flat2d::GameData *gameData, const MapTileObject *tile)
{
	if (gameData->getRenderData()->getCamera()->isVisibleOnCamera(tile->getEntityProperties().getBoundingBox())) {
		spawnTime = true;
	}
	EnemyGenerator::preMove(gameData, tile);
}
