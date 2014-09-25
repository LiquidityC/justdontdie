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

	// Prototype stuff, shouldn't be here in the future
	SDL_Renderer* renderer = window.getRenderer();
	flat2d::GameObject* bot = new Bot(200, 200);
	bot->init(renderer);
	flat2d::ObjectContainer& objectContainer = CompContainer::getInstance().getObjectContainer();
	objectContainer.registerObject(bot);

	// Make a proto floor
	for (int i = 0; i < MAP_WIDTH; i+=10) {
		objectContainer.registerObject(new Block(i, MAP_HEIGHT - 10));
	}
	for (int i = 0; i < MAP_HEIGHT; i+=10) {
		objectContainer.registerObject(new Block(0, i));
		objectContainer.registerObject(new Block(MAP_WIDTH - 10, i));
	}

	SDL_Texture* bgTexture = flat2d::MediaUtil::loadTexture("resources/background.png", renderer);

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
		SDL_RenderCopy(renderer, bgTexture, &cameraBox, NULL);

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
