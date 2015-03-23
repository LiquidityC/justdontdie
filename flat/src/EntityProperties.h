#ifndef ENTITYPROPERTIES_H_
#define ENTITYPROPERTIES_H_

#include <SDL2/SDL.h>
#include <vector>
#include <functional>

#include "Square.h"
#include "MapArea.h"
#include "EntityShape.h"

namespace flat2d
{
	class EntityContainer;

	class EntityProperties : public Square
	{
		public:
			typedef std::function<void ()> OnLocationChangeFunction;
			typedef std::vector<MapArea> Areas;

		private:
			float xvel = 0.0f;
			float yvel = 0.0f;

			bool collidable = false;
			EntityShape colliderShape = { 0, 0, 0, 0 };

			OnLocationChangeFunction onLocationChange = nullptr;
			Areas currentAreas;

		private:
			void locationUpdated();

		public:
			EntityProperties(int x, int y, int dim) : Square(x, y, dim) { }
			EntityProperties(int x, int y, int w, int h) : Square(x, y, w, h) { }

			SDL_Rect getBoundingBox() const;

			EntityShape getColliderShape() const;
			void setColliderShape(EntityShape shape);

			void setCollidable(bool isCollidable);
			bool isCollidable() const;

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

			void setXvel(float xvel);
			float getXvel() const;

			void setYvel(float yvel);
			float getYvel() const;

			bool isMoving() const;

			bool containsPoint(int, int) const;

			Areas& getCurrentAreas();
			const Areas& getCurrentAreas() const;

			void setOnLocationChange(OnLocationChangeFunction);
	};
} // namespace flat2d

#endif // ENTITYPROPERTIES_H_
