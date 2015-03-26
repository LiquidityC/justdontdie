#ifndef COLLISIONDETECTOR_H_
#define COLLISIONDETECTOR_H_

#include "EntityProperties.h"

namespace flat2d
{
	class EntityContainer;
	class Entity;
	class DeltatimeMonitor;

	class CollisionDetector
	{
		private:
			EntityContainer *entityContainer;
			DeltatimeMonitor *dtMonitor;

		public:
			CollisionDetector(EntityContainer *ec, DeltatimeMonitor *dtm) : entityContainer(ec), dtMonitor(dtm) { }

			void moveAllObjects();

			void handlePossibleCollision(Entity*, Entity*) const;

			/**
			 * Collision algorithms follow below
			 *
			 * These are public for the sake of testing
			 */
			bool AABB(const EntityShape&, const EntityShape&) const;

			float sweptAABB(const EntityProperties& props1, const EntityProperties& props2,
					float *normalx, float *normaly) const;

		private:
			CollisionDetector(const CollisionDetector&); // Don't implement, single instance
			const CollisionDetector& operator=(const CollisionDetector& c); // Don't use, single instance

			void handlePossibleCollisionsFor(Entity* entity) const;
	};
} // namespace flat2d

#endif // COLLISIONDETECTOR_H_
