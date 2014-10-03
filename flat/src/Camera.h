#ifndef _CAMERA_H
#define _CAMERA_H

#include <SDL2/SDL.h>

namespace flat2d
{
	class Camera
	{
		private:
			int xpos, ypos, width, height, mapWidth, mapHeight;
			int currentTime = 0;
			int oldTime = 0;
			float deltaTime = 1.0;

		public:
			Camera(int w, int h) :
				xpos(0), 
				ypos(0), 
				width(w), 
				height(h),
				mapWidth(1920), 
				mapHeight(1080) { };

			int getXpos();
			int getYpos();
			int getWidth();
			int getHeight();

			void setMapDimensions(int, int);

			SDL_Rect getBox();

			void centerOn(int x, int y);

			bool isVisibleOnCamera(SDL_Rect& box);

			int getScreenXposFor(int x) const;
			int getScreenYposFor(int y) const;

			void updateDeltaTime();

			float getDeltaTime() const;
	};
}

#endif
