#include <iostream>
#include <SDL2/SDL.h>

#include "timer.h"

const int SCREEN_WIDTH 				= 800;
const int SCREEN_HEIGHT 			= 600;
const int SCREEN_FPS 				= 20;
const int SCREEN_TICKS_PER_FRAME 	= 1000 / SCREEN_FPS;

int main( int argc, char* args[] )
{
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

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
	screenSurface = SDL_GetWindowSurface( window );

	bool quit = false;
	SDL_Event e;

	Timer fpsCapTimer;

	// Main loop
	while (!quit) {

		fpsCapTimer.start();
		
		// Handle events
		while (SDL_PollEvent (&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}
		}

		// Update the screen
		SDL_UpdateWindowSurface( window );

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
