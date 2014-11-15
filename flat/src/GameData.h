#ifndef _GAME_DATA_H
#define _GAME_DATA_H

namespace flat2d
{
	class ObjectContainer;
	class CollisionDetector;
	class RenderData;
	class Mixer;
	class Camera;
	class DeltatimeMonitor;

	class GameData
	{
		private:
			ObjectContainer *objectContainer;
			CollisionDetector *collisionDetector;
			Mixer *mixer;
			Camera *camera;
			DeltatimeMonitor *deltatimeMonitor;
			void *customGameData = nullptr;

		public:
			GameData(ObjectContainer *obc, CollisionDetector *cd, Mixer *m, Camera *c, DeltatimeMonitor *dtm) : 
				objectContainer(obc), 
				collisionDetector(cd),
				mixer(m),
				camera(c),
				deltatimeMonitor(dtm) { };

			ObjectContainer* getObjectContainer() const { return objectContainer; };
			CollisionDetector* getCollisionDetector() const { return collisionDetector; };
			Mixer* getMixer() const { return mixer; };

			void setCustomGameData(void *customGameData) { this->customGameData = customGameData; };
			void* getCustomGameData() const { return customGameData; };
			DeltatimeMonitor* getDeltatimeMonitor() const { return deltatimeMonitor; };
			Camera* getCamera() const { return camera; };
	};
}


#endif
