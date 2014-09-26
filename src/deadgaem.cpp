#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <flat/Timer.h>
#include <flat/ObjectContainer.h>
#include <flat/Window.h>
#include <flat/MediaUtil.h>

#include "CompContainer.h"
#include "Bot.h"
#include "Block.h"
#include "GameSettings.h"
#include "MapParser.h"

int main( int argc, char* args[] )
{

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		std::cerr << "Failed to init video: " 
			<< SDL_GetError() << std::endl;
		return -1;
	}

	flat::Window window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!window.init()) {
		return -1;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init( imgFlags ) & imgFlags )) {
		std::cerr << "Unable to initialize SDL_image: " << IMG_GetError() << std::endl;
		return -1;
	}

	SDL_Renderer* renderer = window.getRenderer();
	flat2d::ObjectContainer& objectContainer = CompContainer::getInstance().getObjectContainer();

	// Prototype stuff, shouldn't be here in the future
	// {{{
	flat2d::GameObject* bot = new Bot(200, 200);
	bot->init(renderer);
	objectContainer.registerObject(bot);

	for (int i = 0; i < MAP_WIDTH; i+=10) {
		Block* b = new Block(i, MAP_HEIGHT - 10);
		b->init(renderer);
		objectContainer.registerObject(b);

		b = new Block(i, 0);
		b->init(renderer);
		objectContainer.registerObject(b);
	}
	for (int i = 0; i < MAP_HEIGHT; i+=10) {
		Block* b = new Block(0, i);
		b->init(renderer);
		objectContainer.registerObject(b);

		b = new Block(MAP_WIDTH - 10, i);
		b->init(renderer);
		objectContainer.registerObject(b);
	}

	MapParser parser;
	parser.createMapFrom("resources/map1/", "map1.tmx", objectContainer);

	SDL_Texture* bgTexture = flat2d::MediaUtil::loadTexture("resources/background.png", renderer);
	// }}}

	// Loop stuff
	flat2d::Timer fpsCapTimer;
	SDL_Event e;
	bool quit = false;
	Camera& camera = CompContainer::getInstance().getCamera();

	// Main loop
	while (!quit) {
		fpsCapTimer.start();

		// Handle events
		objectContainer.preHandle();
		while (SDL_PollEvent (&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}
			objectContainer.handle(e);
		}
		objectContainer.postHandle();

		// Clear screen to black
		SDL_SetRenderDrawColor( renderer, 0x0, 0x0, 0x0, 0xFF );
		SDL_RenderClear( renderer );

		SDL_Rect cameraBox = camera.getBox();
		SDL_Rect gameBox = { 0, 0, MAP_WIDTH, MAP_HEIGHT };
		gameBox.x -= cameraBox.x;
		gameBox.y -= cameraBox.y;
		SDL_RenderCopy(renderer, bgTexture, NULL, &gameBox);

		// Render
		objectContainer.preRender();
		objectContainer.render(renderer);
		objectContainer.postRender();

		// Update the screen
		SDL_RenderPresent( renderer );

		// Cap the frame rate
		int frameTicks = fpsCapTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
		}
		fpsCapTimer.stop();
	}

	SDL_DestroyTexture( bgTexture );

	IMG_Quit();
	SDL_Quit();

	return 0;
}
