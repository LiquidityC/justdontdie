#ifndef _COLLISION_DETECTOR_H
#define _COLLISION_DETECTOR_H

namespace flat2d 
{
	class EntityContainer;
	class Entity;

	class CollisionDetector
	{

		private:
			EntityContainer* entityContainer;

		public:
			CollisionDetector(EntityContainer* entityContainer);

			Entity* checkForCollisions(const Entity*) const;
			bool hasCollided(const Entity*, const Entity*) const;

		private:
			CollisionDetector(const CollisionDetector&); // Don't implement, single instance
			const CollisionDetector& operator=(const CollisionDetector& c); // Don't use, single instance
	};
}

#endif
