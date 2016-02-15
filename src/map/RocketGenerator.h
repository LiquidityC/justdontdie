#ifndef MAP_ROCKETGENERATOR_H_
#define	MAP_ROCKETGENERATOR_H_

#include <flat/flat.h>
#include "EntityGenerator.h"

class MapTileObject;

class RocketGenerator : public EntityGenerator
{
	private:
		flat2d::Timer launchTimer;

	public:
		void preMove(const flat2d::GameData *gameData, const MapTileObject *tile);
};

#endif // MAP_ROCKETGENERATOR_H_
