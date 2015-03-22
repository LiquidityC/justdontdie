#ifndef _LOCATION_PROPERTY_H
#define _LOCATION_PROPERTY_H

#include <SDL2/SDL.h>
#include <vector>
#include <functional>

namespace flat2d
{
	class EntityContainer;

	class EntityProperties
	{
		public:
			typedef std::function<void ()> OnLocationChangeFunction;
			typedef std::vector<EntityProperties> Parents;

		private:
			int xpos, ypos, xvel, yvel, width, height;
			OnLocationChangeFunction onLocationChange = nullptr;
			Parents parents;

		private:
			void locationUpdated();

		public:
			EntityProperties(int x, int y, int dim) : xpos(x), ypos(y), width(dim), height(dim) { };
			EntityProperties(int x, int y, int w, int h) : xpos(x), ypos(y), width(w), height(h) { };

			SDL_Rect getBoundingBox() const;

			void setXpos(int x);
			void incrementXpos(int x);
			int getXpos() const;

			void setYpos(int y);
			void incrementYpos(int y);
			int getYpos() const;

			void setWidth(int w);
			int getWidth() const;

			void setHeight(int h);
			int getHeight() const;

			bool containsPoint(int, int) const;

			Parents& getParents();
			const Parents& getParents() const;

			bool operator<(const EntityProperties&) const;
			bool operator==(const EntityProperties&) const;
			bool operator!=(const EntityProperties&) const;

			void setOnLocationChange(OnLocationChangeFunction);
	};
}

#endif
