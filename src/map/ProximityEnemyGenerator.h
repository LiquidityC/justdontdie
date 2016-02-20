#ifndef MAP_PROXIMITYENEMYGENERATOR_H_
#define	MAP_PROXIMITYENEMYGENERATOR_H_

#include "EnemyGenerator.h"

class ProximityEnemyGenerator : public EnemyGenerator
{
	private:
		bool hasSpawned = false;
		bool spawnTime = false;

		bool timeToSpawn();

	public:
		void preMove(const flat2d::GameData *gameData, const MapTileObject *tile);
};


#endif // MAP_PROXIMITYENEMYGENERATOR_H_
