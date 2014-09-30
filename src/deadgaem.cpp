#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

	flat::Window window(GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT);
	if (!window.init()) {
		return -1;
	}

	SDL_Renderer* renderer = window.getRenderer();
	flat2d::ObjectContainer& objectContainer = CompContainer::getInstance().getObjectContainer();

	// Prototype stuff, shouldn't be here in the future
	// {{{
	MapParser parser;
	parser.createMapFrom("resources/map2/", "map2.tmx", renderer);
	
	flat2d::GameObject* bot = new Bot(200, 200);
	bot->init(renderer);
	objectContainer.registerObject(bot, Layers::MID);
	// }}}

	// Loop stuff
	flat2d::Timer fpsCapTimer;
	SDL_Event e;
	bool quit = false;

	// Main loop
	CompContainer::getInstance().getCamera().updateDeltaTime();
	while (!quit) {
		CompContainer::getInstance().getCamera().updateDeltaTime();
		fpsCapTimer.start();

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
		objectContainer.preRenderObjects();
		objectContainer.renderObjects(renderer);
		objectContainer.postRenderObjects();

		// Update the screen
		SDL_RenderPresent( renderer );

		// Cap the frame rate
		int frameTicks = fpsCapTimer.getTicks();
		if (frameTicks < GameSettings::SCREEN_TICKS_PER_FRAME) {
			SDL_Delay( GameSettings::SCREEN_TICKS_PER_FRAME - frameTicks );
		}
		fpsCapTimer.stop();
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}
