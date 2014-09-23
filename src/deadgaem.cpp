#include <iostream>
#include <SDL2/SDL.h>
#include <flat-2d/Timer.h>
#include <flat-2d/ObjectContainer.h>

#include "CompContainer.h"
#include "Ship.h"

const int SCREEN_WIDTH 				= 800;
const int SCREEN_HEIGHT 			= 600;
const int SCREEN_FPS 				= 60;
const int SCREEN_TICKS_PER_FRAME 	= 1000 / SCREEN_FPS;

int main( int argc, char* args[] )
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		std::cerr << "Failed to init video: " 
			<< SDL_GetError() << std::endl;
		return -1;
	}

	window = SDL_CreateWindow( "DeadGaem", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, 
			SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN );

	if (window == NULL) {
		std::cerr << "Window could not be created: " 
			<< SDL_GetError() << std::endl;
		return -1;
	}

	SDL_ShowCursor(0);
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if (renderer == NULL) {
		std::cerr << "Renderer could not be created: " << SDL_GetError() << std::endl;
	}

	bool quit = false;
	SDL_Event e;

	Timer fpsCapTimer;

	GameObject* ship = new Ship(200, 200);
	ObjectContainer& objectContainer = CompContainer::getInstance().getObjectContainer();
	objectContainer.registerObject(ship);

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

		// Render
		objectContainer.preRender();
		objectContainer.render(renderer);
		objectContainer.postRender();

		// Update the screen
		SDL_RenderPresent( renderer );

		if (CompContainer::getInstance().getCollisionDetector().checkForCollisions(ship)) {
			break;
		}

		CompContainer::getInstance().getPathGenerator().generatePath(SCREEN_WIDTH, SCREEN_HEIGHT);
		objectContainer.cleanNonVisibleObjects(SCREEN_WIDTH, SCREEN_HEIGHT);

		// Cap the frame rate
		int frameTicks = fpsCapTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
		}
		fpsCapTimer.stop();
	}

	SDL_DestroyWindow( window );

	SDL_Quit();

	return 0;
}
