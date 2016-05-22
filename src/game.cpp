#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <flat/flat.h>
#include <iostream>
#include <string>
#include <sstream>

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

	std::stringstream ss;
	ss << "Just Don't Die " << VERSION_MAJOR << "." << VERSION_MINOR;
#ifdef DEBUG
	ss << " [DEBUG]";
#endif // DEBUG

	if (!flat->initSDL(ss.str(), GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT)) {
		return -1;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	if (!flat->initContainers()) {
		return -1;
	}
	if (SDL_RenderSetLogicalSize(flat->getGameData()->getRenderData()->getRenderer(),
			GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT)) {
		return -1;
	}

	GameStateController *gameStateController = new GameStateController();
	gameStateController->resetGame(flat->getGameData());

	flat2d::GameEngine *engine = flat->getGameEngine();
	engine->init(GameSettings::SCREEN_FPS);

	// Start the game loop
	engine->run(gameStateController);

	delete static_cast<CustomGameData*>(flat->getGameData()->getCustomGameData());
	delete flat;

	return 0;
}
