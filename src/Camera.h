#ifndef _CAMERA_H
#define _CAMERA_H

#include <SDL2/SDL.h>
#include "GameSettings.h"

class Camera
{
	private:
		int xpos, ypos, width, height;

	public:
		Camera() : xpos(0), ypos(0), width(SCREEN_WIDTH), height(SCREEN_HEIGHT) { };

		int getXpos();
		int getYpos();
		int getWidth();
		int getHeight();

		SDL_Rect getBox();

		void centerOn(int x, int y);

		bool isVisibleOnCamera(SDL_Rect& box);

		int getScreenXposFor(int x);
		int getScreenYposFor(int y);
};

#endif
