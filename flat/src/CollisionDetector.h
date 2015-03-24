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

			Entity* checkForCollisions(const Entity*) const;
			bool hasCollided(const Entity*, const Entity*) const;

		private:
			CollisionDetector(const CollisionDetector&); // Don't implement, single instance
			const CollisionDetector& operator=(const CollisionDetector& c); // Don't use, single instance

			bool AABB(const EntityProperties& props1, const EntityProperties& props2) const;

			float sweptAABB(const EntityProperties& props1, const EntityProperties& props2,
					float *normalx, float *normaly) const;
	};
} // namespace flat2d

#endif // COLLISIONDETECTOR_H_
