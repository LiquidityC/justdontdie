#ifndef _WINDOW_H
#define _WINDOW_H

#include <SDL2/SDL.h>
#include <cstdio>
#include <string>

namespace flat2d
{
	class Window
	{
		private:
			std::string title;
			unsigned int width, height;

			SDL_Window* window;
			SDL_Renderer* renderer;

		public:
			Window(std::string t, unsigned int w, unsigned int h) : title(t), width(w), height(h) { };

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
