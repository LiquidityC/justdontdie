#ifndef MAP_ENTITYGENERATOR_H_
#define	MAP_ENTITYGENERATOR_H_

#include <flat/flat.h>

class MapTileObject;

class EntityGenerator
{
	public:
		virtual void preMove(const flat2d::GameData *gameData, const MapTileObject *tile) = 0;
		virtual ~EntityGenerator() { }
};

#endif // MAP_ENTITYGENERATOR_H_
