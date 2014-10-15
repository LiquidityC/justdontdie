#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <flat/flat.h>

#include "Soldier.h"
#include "ParticleEngine.h"
#include "ResourceContainer.h"
#include "GameSettings.h"
#include "MapParser.h"
#include "Layers.h"

int main( int argc, char* args[] )
{
	flat2d::FlatBuilder *flat = new flat2d::FlatBuilder;
	if (!flat->initSDL("DeadGaem", GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT)) {
		return -1;
	}
	if (!flat->initContainers()) {
		return -1;
	}

	flat2d::RenderData* renderData = flat->getRenderData();
	SDL_Renderer* renderer = renderData->getRenderer();
	flat2d::Camera* camera = renderData->getCamera();
	flat2d::GameData *gameData = flat->getGameData();
	flat2d::ObjectContainer* objectContainer = gameData->getObjectContainer();
	ParticleEngine *particleEngine = new ParticleEngine(objectContainer);
	ResourceContainer *resourceContainer = new ResourceContainer();

	objectContainer->addLayer(Layers::BACK);
	objectContainer->addLayer(Layers::MID);
	objectContainer->addLayer(Layers::FRONT);

	// Prototype stuff, shouldn't be here in the future
	// {{{
	MapParser parser;
	parser.createMapFrom(resourceContainer, "resources/maps/map1/", "map1.tmx", renderData);
	
	flat2d::GameObject* soldier = new Soldier(particleEngine, 200, 200);
	soldier->init(renderData);
	objectContainer->registerObject(soldier, Layers::MID);

	flat2d::Timer fpsTimer;
	flat2d::Timer drawFpsTimer;
	int countedFrames = 0;
	int currentFps = 0;
	float avgFps = 0;
	fpsTimer.start();
	drawFpsTimer.start();
	// }}}

	// Loop stuff
	flat2d::Timer fpsCapTimer;;
	SDL_Event e;
	bool quit = false;

	// Main loop
	camera->updateDeltaTime();
	while (!quit) {
		fpsCapTimer.start();
		camera->updateDeltaTime();

		// Handle events
		objectContainer->preHandleObjects(gameData);
		while (SDL_PollEvent (&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}
			objectContainer->handleObjects(e);
		}
		objectContainer->postHandleObjects(gameData);

		// Clear screen to black
		SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF );
		SDL_RenderClear( renderer );

		// Render
		objectContainer->preRenderObjects(renderData);
		objectContainer->renderObjects(renderData);
		objectContainer->postRenderObjects(renderData);

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

	delete particleEngine;
	delete resourceContainer;
	delete flat;
	IMG_Quit();
	SDL_Quit();

	return 0;
}
