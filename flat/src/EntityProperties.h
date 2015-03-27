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
			typedef std::vector<MapArea> Areas;

		private:
			float xvel = 0.0f;
			float yvel = 0.0f;

			bool collidable = false;
			bool locationChanged = false;

			EntityShape colliderShape = { 0, 0, 0, 0 };

			Areas currentAreas;

		public:
			EntityProperties(int x, int y, int dim) : Square(x, y, dim) { }
			EntityProperties(int x, int y, int w, int h) : Square(x, y, w, h) { }

			SDL_Rect getBoundingBox() const;

			EntityShape getColliderShape() const;
			void setColliderShape(EntityShape shape);

			EntityShape getVelocityColiderShape(float deltatime) const;

			void setCollidable(bool isCollidable);
			bool isCollidable() const;

			void incrementXpos(int x);
			void setXpos(int pos);
			int getXpos() const;
			void incrementYpos(int y);
			void setYpos(int pos);
			int getYpos() const;
			int getWidth() const;
			int getHeight() const;
			void setXvel(float xvel);
			float getXvel() const;
			void setYvel(float yvel);
			float getYvel() const;

			bool isMoving() const;

			void move(float deltatime);

			bool containsPoint(int, int) const;

			Areas& getCurrentAreas();
			const Areas& getCurrentAreas() const;

			void setLocationChanged(bool changed);
			bool hasLocationChanged() const;
	};
} // namespace flat2d

#endif // ENTITYPROPERTIES_H_
