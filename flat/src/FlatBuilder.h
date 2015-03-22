#ifndef FLATBUILDER_H_
#define FLATBUILDER_H_

#include <string>

namespace flat2d
{
	class EntityContainer;
	class CollisionDetector;
	class Window;
	class Camera;
	class GameData;
	class RenderData;
	class Mixer;
	class DeltatimeMonitor;
	class GameControllerContainer;

	class FlatBuilder
	{
		private:
			Window *window = nullptr;
			RenderData *renderData = nullptr;
			GameData *gameData = nullptr;
			CollisionDetector *collisionDetector = nullptr;
			EntityContainer *entityContainer = nullptr;
			Camera *camera = nullptr;
			Mixer *mixer = nullptr;
			DeltatimeMonitor *deltatimeMonitor = nullptr;
			GameControllerContainer *controllerContainer = nullptr;

		public:
			~FlatBuilder();

			bool initSDL(std::string, int, int);
			bool initContainers();

			GameData* getGameData() const;
	};
} // namespace flat2d

#endif // FLATBUILDER_H_
