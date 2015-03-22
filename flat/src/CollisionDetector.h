#ifndef COLLISIONDETECTOR_H_
#define COLLISIONDETECTOR_H_

namespace flat2d
{
	class EntityContainer;
	class Entity;

	class CollisionDetector
	{
		private:
			EntityContainer* entityContainer;

		public:
			explicit CollisionDetector(EntityContainer* entityContainer);

			Entity* checkForCollisions(const Entity*) const;
			bool hasCollided(const Entity*, const Entity*) const;

		private:
			CollisionDetector(const CollisionDetector&); // Don't implement, single instance
			const CollisionDetector& operator=(const CollisionDetector& c); // Don't use, single instance
	};
} // namespace flat2d

#endif // COLLISIONDETECTOR_H_
