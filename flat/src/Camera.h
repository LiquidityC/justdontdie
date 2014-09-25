#ifndef _CAMERA_H
#define _CAMERA_H

#include <SDL2/SDL.h>

class Camera
{
	private:
		int xpos, ypos, width, height, mapWidth, mapHeight;

	public:
		Camera(int x, int y, int w, int h, int mw, int mh) :
			xpos(x), 
			ypos(y), 
			width(w), 
			height(h),
			mapWidth(mw), 
			mapHeight(mh) { };

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
