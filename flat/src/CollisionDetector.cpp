#include "Entity.h"
#include "CollisionDetector.h"
#include "EntityContainer.h"

namespace flat2d
{
	CollisionDetector::CollisionDetector(EntityContainer* entityContainer)
	{
		this->entityContainer = entityContainer;
	}

	Entity* CollisionDetector::checkForCollisions(const Entity* o1) const
	{
		return entityContainer->checkCollidablesFor(o1,
				[this, o1](const Entity* o2)
				{
				return this->hasCollided(o1, o2);
				});
	}

	bool CollisionDetector::hasCollided(const Entity* o1, const Entity* o2) const
	{
		if (*o1 == *o2) {
			return false;
		}

		EntityShape b1 = o1->getEntityProperties().getColliderShape();
		EntityShape b2 = o2->getEntityProperties().getColliderShape();

		if (b1.x > b2.x + b2.w) {
			return false;
		} else if (b1.x + b1.w < b2.x) {
			return false;
		} else if (b1.y > b2.y + b2.h) {
			return false;
		} else if (b1.y + b1.h < b2.y) {
			return false;
		}

		return true;
	}
} // namespace flat2d
