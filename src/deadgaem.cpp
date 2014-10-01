#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <flat/flat.h>

#include "CompContainer.h"
#include "Bot.h"
#include "GameSettings.h"
#include "MapParser.h"

int main( int argc, char* args[] )
{

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		std::cerr << "Failed to init video: " 
			<< SDL_GetError() << std::endl;
		return -1;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init( imgFlags ) & imgFlags )) {
		std::cerr << "Unable to initialize SDL_image: " << IMG_GetError() << std::endl;
		return -1;
	}

	if (TTF_Init() == -1) {
		std::cerr << "Unable to initiate SDL2_ttf: " << TTF_GetError() << std::endl;
		return -1;
	}

	flat::Window window(GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT);
	if (!window.init()) {
		return -1;
	}

	SDL_Renderer* renderer = window.getRenderer();
	flat2d::ObjectContainer& objectContainer = CompContainer::getInstance().getObjectContainer();
	flat2d::Camera& camera = CompContainer::getInstance().getCamera();
	flat2d::RenderData renderData(renderer, &camera);

	// Prototype stuff, shouldn't be here in the future
	// {{{
	MapParser parser;
	parser.createMapFrom("resources/map2/", "map2.tmx", renderer);
	
	flat2d::GameObject* bot = new Bot(200, 200);
	bot->init(&renderData);
	objectContainer.registerObject(bot, Layers::MID);

	std::stringstream timeText;
	flat2d::Timer fpsTimer;
	flat2d::Timer drawFpsTimer;
	int countedFrames = 0;
	fpsTimer.start();
	drawFpsTimer.start();
	// }}}

	// Loop stuff
	flat2d::Timer fpsCapTimer;;
	SDL_Event e;
	bool quit = false;

	// Main loop
	camera.updateDeltaTime();
	while (!quit) {
		fpsCapTimer.start();
		camera.updateDeltaTime();

		// Handle events
		objectContainer.preHandleObjects();
		while (SDL_PollEvent (&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}
			objectContainer.handleObjects(e);
		}
		objectContainer.postHandleObjects();

		// Clear screen to black
		SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF );
		SDL_RenderClear( renderer );

		// Render
		objectContainer.preRenderObjects(&renderData);
		objectContainer.renderObjects(&renderData);
		objectContainer.postRenderObjects(&renderData);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

		// Update the screen
		SDL_RenderPresent( renderer );

#ifdef DEBUG
		countedFrames++;
		float avgFps = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFps > 20000) {
			avgFps = 0;
		}
		timeText.str("");
		timeText << "FPS: " << avgFps;
		if (static_cast<int>((drawFpsTimer.getTicks() / 1000.f)) > 1) {
			std::cout << timeText.str() << std::endl;
			drawFpsTimer.stop();
			drawFpsTimer.start();
		}
#endif
		int tickCount = fpsCapTimer.getTicks();
		if (tickCount < GameSettings::SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(GameSettings::SCREEN_TICKS_PER_FRAME - tickCount);
		}
	}

	objectContainer.unregisterAllObjects();
	CompContainer::getInstance().getResourceContainer().clearTextures();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
