#ifndef GAMESTATECONTROLLER_H_
#define	GAMESTATECONTROLLER_H_

#include <flat/flat.h>
#include <string>
#include <vector>

#include "GameStates.h"

class GameStateController
{
	typedef struct {
		std::string path;
		std::string file;
	} MapData;

	private:
		bool reset = false;

		GameState currentState = SPLASH;
		flat2d::Timer splashTimer;

		size_t currentMapIndex = 0;
		std::vector<MapData> maps;

		void initMaps();

		void clearAllAssets(flat2d::GameData *gameData);
		void loadSplash(flat2d::GameData *gameData);
		void loadGame(flat2d::GameData *gameData);

	public:
		GameStateController();
		virtual ~GameStateController() { }

		// Overrides
		void handle(const SDL_Event& event);
		bool gameStateCheck(flat2d::GameData *gameData);
		bool quit();

		void resetGame(flat2d::GameData *gameData);

		void loadNextMap();
};

#endif // GAMESTATECONTROLLER_H_
