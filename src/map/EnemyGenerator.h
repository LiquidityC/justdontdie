#ifndef MAP_ENEMYGENERATOR_H_
#define	MAP_ENEMYGENERATOR_H_

#include <flat/flat.h>
#include "EntityGenerator.h"

class MapTileObject;

class EnemyGenerator : public EntityGenerator
{
	private:
		const int defaultSpawnTime = 5;

		flat2d::Timer spawnTimer;

	public:
		void preMove(const flat2d::GameData *gameData, const MapTileObject *tile);
};

#endif // MAP_ENEMYGENERATOR_H_
