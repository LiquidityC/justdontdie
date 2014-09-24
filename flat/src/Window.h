#ifndef _WINDOW_H
#define _WINDOW_H

#include <SDL2/SDL.h>
#include <cstdio>

namespace flat
{
	class Window
	{
		private:
			unsigned int width, height;

			SDL_Window* window;
			SDL_Renderer* renderer;

		public:
			Window(unsigned int w, unsigned int h) : width(w), height(h) { };

			~Window() {
				SDL_DestroyWindow(window);
				SDL_DestroyRenderer(renderer);

				window = NULL;
				renderer = NULL;
			};

			bool init();

			SDL_Window* getWindow() { return window; };
			SDL_Renderer* getRenderer() { return renderer; };
	};
}

#endif
