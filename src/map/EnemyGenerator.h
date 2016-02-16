#ifndef MAP_ENEMYGENERATOR_H_
#define	MAP_ENEMYGENERATOR_H_

#include <flat/flat.h>
#include "EntityGenerator.h"

class MapTileObject;

class EnemyGenerator : public EntityGenerator
{
	protected:
		virtual bool timeToSpawn() = 0;

	public:
		virtual void preMove(const flat2d::GameData *gameData, const MapTileObject *tile);
};

#endif // MAP_ENEMYGENERATOR_H_
