#include <limits>
#include <algorithm>

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

		return AABB(o1->getEntityProperties(), o2->getEntityProperties());

		/*
		 * Will use this later when velocities are treated with deltatime
		 *
		if (!o1->getEntityProperties().isMoving()) {
			return AABB(o1->getEntityProperties(), o2->getEntityProperties());
		} else {
			float normalx, normaly;
			float res = sweptAABB(o1->getEntityProperties(), o2->getEntityProperties(), &normalx, &normaly);
			return res > 0.0f && res < 1.0f;
		}
		*/
	}

	bool CollisionDetector::AABB(const EntityProperties& p1, const EntityProperties& p2) const
	{
		EntityShape b1 = p1.getColliderShape();
		EntityShape b2 = p2.getColliderShape();

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

	float CollisionDetector::sweptAABB(const EntityProperties& p1, const EntityProperties& p2,
			float *normalx, float *normaly) const
	{
		EntityShape b1 = p1.getColliderShape();
		EntityShape b2 = p2.getColliderShape();

		float xvel = p1.getXvel();
		float yvel = p1.getYvel();


		// Find the distances to the blocking object
		float xInvEntry, xInvExit;
		float yInvEntry, yInvExit;

		if (xvel > 0.0f) {
			xInvEntry = b2.x - (b1.x + b1.w);
			xInvExit = (b2.x + b2.w) - b1.x;
		} else {
			xInvEntry = (b2.x + b2.w) - b1.x;
			xInvExit = b2.x - (b1.x + b1.w);
		}

		if (yvel > 0.0f) {
			yInvEntry = b2.y - (b1.y + b1.w);
			yInvExit = (b2.y + b2.w) - b1.y;
		} else {
			yInvEntry = (b2.y + b2.w) - b1.y;
			yInvExit = b2.y - (b1.y + b1.w);
		}

		// Find the impact time
		float xEntry, xExit;
		float yEntry, yExit;

		if (xvel == 0.0f) {
			xEntry = -std::numeric_limits<float>::infinity();
			xExit = -std::numeric_limits<float>::infinity();
		} else {
			xEntry = xInvEntry / xvel;
			xExit = xInvExit / xvel;
		}

		if (yvel == 0.0f) {
			yEntry = -std::numeric_limits<float>::infinity();
			yExit = -std::numeric_limits<float>::infinity();
		} else {
			yEntry = yInvEntry / yvel;
			yExit = yInvExit / yvel;
		}

		// Find earliest/latest times of collition
		float entryTime = std::max(xEntry, yEntry);
		float exitTime = std::min(xExit, yExit);

		if (entryTime < exitTime || (xEntry < 0.0f && yEntry < 0.0f) || xEntry > 1.0f || yEntry > 1.0f ) {
			// No collision
			*normalx = 0.0f;
			*normaly = 0.0f;
			return 1.0f;
		} else {
			if (xEntry > yEntry) {
				if (xInvEntry < 0.0f) {
					*normalx = 1.0f;
					*normaly = 0.0f;
				} else {
					*normalx = -1.0f;
					*normaly = 0.0f;
				}
			} else {
				if (yInvEntry < 0.0f) {
					*normalx = 0.0f;
					*normaly = 1.0f;
				} else {
					*normalx = 0.0f;
					*normaly = -1.0f;
				}
			}
		}
		return entryTime;
	}
} // namespace flat2d
