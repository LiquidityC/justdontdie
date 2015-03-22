#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <flat/flat.h>
#include <iostream>

#include "Soldier.h"
#include "ParticleEngine.h"
#include "ResourceContainer.h"
#include "GameSettings.h"
#include "MapParser.h"
#include "LayerService.h"
#include "ResourceLoader.h"
#include "CustomGameData.h"

int main( int argc, char* args[] )
{
	flat2d::FlatBuilder *flat = new flat2d::FlatBuilder;
	if (!flat->initSDL("DeadGaem", GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT)) {
		return -1;
	}
	if (!flat->initContainers()) {
		return -1;
	}

	// TODO(Linus): Should probably extract all this into a function/method {{
	flat2d::GameData *gameData = flat->getGameData();
	flat2d::RenderData* renderData = gameData->getRenderData();
	SDL_Renderer* renderer = renderData->getRenderer();
	flat2d::EntityContainer* entityContainer = gameData->getEntityContainer();

	CustomGameData *customGameData = CustomGameData::create(gameData);
	gameData->setCustomGameData(customGameData);

	customGameData->getLayerService()->registerLayers(entityContainer);

	MapParser parser;
	parser.createMapFrom(gameData, "resources/maps/map1/", "map1.tmx");

	flat2d::Entity* soldier = new Soldier(200, 200);
	soldier->init(gameData);
	entityContainer->registerObject(soldier, Layers::MID);

	ResourceLoader *rLoader = customGameData->getResourceLoader();
	rLoader->loadMusic(gameData);
	rLoader->loadSoundEffects(gameData);
	// }}

	flat2d::Timer fpsTimer;
	flat2d::Timer drawFpsTimer;
	int countedFrames = 0;
	int currentFps = 0;
	float avgFps = 0;
	fpsTimer.start();
	drawFpsTimer.start();

	// Loop stuff
	flat2d::Timer fpsCapTimer;;
	SDL_Event e;
	bool quit = false;

	// Main loop
	gameData->getDeltatimeMonitor()->updateDeltaTime();
	while (!quit) {
		fpsCapTimer.start();
		gameData->getDeltatimeMonitor()->updateDeltaTime();

		// Handle events
		entityContainer->preHandleObjects(gameData);
		while (SDL_PollEvent (&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}
			entityContainer->handleObjects(e);
		}
		entityContainer->postHandleObjects(gameData);

		// Clear screen to black
		SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF );
		SDL_RenderClear( renderer );

		// Render
		entityContainer->preRenderObjects(gameData);
		entityContainer->renderObjects(renderData);
		entityContainer->postRenderObjects(gameData);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

		// Update the screen
		SDL_RenderPresent( renderer );

#ifdef DEBUG
		avgFps = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFps > 20000) {
			avgFps = 0;
		}
		if (static_cast<int>((drawFpsTimer.getTicks() / 1000.f)) > 1) {
			std::cout << "AVG FPS: " << avgFps << " FPS: " << currentFps << std::endl;
			currentFps = 0;
			drawFpsTimer.stop();
			drawFpsTimer.start();
		}
		countedFrames++;
		currentFps++;
#endif

		int tickCount = fpsCapTimer.getTicks();
		if (tickCount < GameSettings::SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(GameSettings::SCREEN_TICKS_PER_FRAME - tickCount);
		}
	}

	delete customGameData;
	delete flat;

	return 0;
}
