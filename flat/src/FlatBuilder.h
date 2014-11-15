#ifndef _FLAT_BUILDER_H
#define _FLAT_BUILDER_H

#include <string>

namespace flat2d
{
	class ObjectContainer;
	class CollisionDetector;
	class Window;
	class Camera;
	class GameData;
	class RenderData;
	class Mixer;
	class DeltatimeMonitor;

	class FlatBuilder
	{
		private:
			Window *window = nullptr;
			RenderData *renderData = nullptr;
			GameData *gameData = nullptr;
			CollisionDetector *collisionDetector = nullptr;
			ObjectContainer *objectContainer = nullptr;
			Camera *camera = nullptr;
			Mixer *mixer = nullptr;
			DeltatimeMonitor *deltatimeMonitor = nullptr;

		public:
			~FlatBuilder();

			bool initSDL(std::string, int, int);
			bool initContainers();

			RenderData* getRenderData() const;
			GameData* getGameData() const;

	};
}

#endif
