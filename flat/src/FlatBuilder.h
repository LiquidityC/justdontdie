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

	class FlatBuilder
	{
		private:
			Window *window = NULL;
			RenderData *renderData = NULL;
			GameData *gameData = NULL;
			CollisionDetector *collisionDetector = NULL;
			ObjectContainer *objectContainer = NULL;
			Camera *camera = NULL;
			Mixer *mixer = NULL;

		public:
			~FlatBuilder();

			bool initSDL(std::string, int, int);
			bool initContainers();

			RenderData* getRenderData() const;
			GameData* getGameData() const;

	};
}

#endif
