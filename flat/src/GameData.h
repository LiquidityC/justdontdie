#ifndef _GAME_DATA_H
#define _GAME_DATA_H

namespace flat2d
{
	class ObjectContainer;
	class CollisionDetector;
	class RenderData;
	class Mixer;

	class GameData
	{
		private:
			ObjectContainer *objectContainer;
			CollisionDetector *collisionDetector;
			Mixer *mixer;

		public:
			GameData(ObjectContainer *obc, CollisionDetector *cd, Mixer *m) : 
				objectContainer(obc), 
				collisionDetector(cd),
				mixer(m) { };

			ObjectContainer* getObjectContainer() const { return objectContainer; };
			CollisionDetector* getCollisionDetector() const { return collisionDetector; };
			Mixer* getMixer() const { return mixer; };
	};
}


#endif
