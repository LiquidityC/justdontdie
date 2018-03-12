#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <flat/flat.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

#include "Soldier.h"
#include "ParticleEngine.h"
#include "ResourceContainer.h"
#include "GameSettings.h"
#include "map/MapParser.h"
#include "LayerService.h"
#include "ResourceLoader.h"
#include "CustomGameData.h"
#include "FrameCounter.h"
#include "Config.h"

int main( int argc, char* args[] )
{
	flat2d::FlatBuilder *flat = new flat2d::FlatBuilder;
	if (argc > 1 && !strcmp(args[1], "--hi-dpi")) {
		flat->setHiDPI(true);
	}

	std::stringstream ss;
	ss << "Just Don't Die " << VERSION_MAJOR << "." << VERSION_MINOR;
#ifdef DEBUG
	ss << " [DEBUG]";
#endif // DEBUG

	int error = flat->loadSDL(ss.str(),
			GameSettings::SCREEN_FPS,
			GameSettings::SCREEN_WIDTH,
			GameSettings::SCREEN_HEIGHT);

	if (error) {
		std::cerr << "Failed to load SDL, exiting" << std::endl;
		return -1;
	}

	GameStateController *gameStateController = new GameStateController();
	gameStateController->resetGame(flat->getGameData());

	// Define our callbacks
	auto stateCheck = [gameStateController](flat2d::GameData *gameData) -> flat2d::GameStateAction
	{
		if (gameStateController->quit()) {
			return flat2d::GameStateAction::QUIT;
		} else if (gameStateController->gameStateCheck(gameData)) {
			return flat2d::GameStateAction::RESET;
		}
		return flat2d::GameStateAction::NOOP;
	};

	auto handleCallback = [gameStateController](const SDL_Event& event)
	{
		gameStateController->handle(event);
	};

	// Start the game loop
	flat->getGameEngine()->run(stateCheck, handleCallback);

	delete static_cast<CustomGameData*>(flat->getGameData()->getCustomGameData());
	delete flat;

	return 0;
}
