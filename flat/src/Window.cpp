#include <iostream>
#include "Window.h"

using namespace flat;

bool Window::init()
{
	window = SDL_CreateWindow( "DeadGaem",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			SDL_WINDOW_SHOWN );

	if (window == NULL) {
		std::cerr << "Window could not be created: "
			<< SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if (renderer == NULL) {
		std::cerr << "Renderer could not be created: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}
