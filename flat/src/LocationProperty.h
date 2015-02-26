#ifndef _LOCATION_PROPERTY_H
#define _LOCATION_PROPERTY_H

#include <SDL2/SDL.h>
#include <vector>

namespace flat2d
{
	class LocationProperty
	{
		private:
			int xpos, ypos, width, height;

		public:
			LocationProperty(int x, int y, int w, int h) : xpos(x), ypos(y), width(w), height(h) {
				 // Do nothing
			}

			SDL_Rect getBoundingBox() const;

			void setXpos(int x) { xpos = x; };
			void incrementXpos(int x) { xpos += x; };
			int getXpos() const { return xpos; };

			void setYpos(int y) { ypos = y; };
			void incrementYpos(int y) { ypos += y; };
			int getYpos() const { return ypos; };

			void setWidth(int w) { width = w; };
			int getWidth() const { return width; };

			void setHeight(int h) { height = h; };
			int getHeight() const { return height; };
	};

	class LocationPropertyListener
	{
		virtual void onPositionChanged(const LocationProperty&) = 0;
	};
}

#endif
