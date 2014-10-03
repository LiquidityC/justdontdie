#ifndef _GAME_DATA_H
#define _GAME_DATA_H

namespace flat2d
{
	class ObjectContainer;
	class CollisionDetector;

	class GameData
	{
		private:
			ObjectContainer *objectContainer;
			CollisionDetector *collisionDetector;

		public:
			GameData(ObjectContainer *obc, CollisionDetector *cd) : 
				objectContainer(obc), 
				collisionDetector(cd) { };

			ObjectContainer* getObjectContainer() const { return objectContainer; };
			CollisionDetector* getCollisionDetector() const { return collisionDetector; };
	};
}


#endif
