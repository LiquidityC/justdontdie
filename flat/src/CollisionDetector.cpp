#include <limits>
#include <algorithm>
#include <vector>

#include "Entity.h"
#include "EntityShape.h"
#include "CollisionDetector.h"
#include "EntityContainer.h"
#include "DeltatimeMonitor.h"
#include "GameData.h"

namespace flat2d
{
	void CollisionDetector::handlePossibleCollisionsFor(Entity* e, const GameData *data) const
	{
		entityContainer->iterateCollidablesFor(e,
				[this, e, data](Entity* o)
				{
					EntityShape broadphaseShape = e->getEntityProperties().getVelocityColiderShape(
							dtMonitor->getDeltaTime());
					if (this->AABB(broadphaseShape, o->getEntityProperties().getColliderShape())) {
						this->handlePossibleCollision(e, o, data);
					}
				});
	}

	void CollisionDetector::handlePossibleCollision(Entity* o1, Entity* o2, const GameData *data) const
	{
		if (*o1 == *o2) {
			return;
		}

		float normalx, normaly;
		EntityProperties &props = o1->getEntityProperties();

		float collisionTime = sweptAABB(&props, &o2->getEntityProperties(), &normalx, &normaly);
		if (collisionTime < 1.0f) {
			bool collisionHandled = o1->onCollision(o2, data);
			o2->onCollision(o1, data);

			if (collisionHandled) {
				return;
			}

			float xvel = props.getXvel() * dtMonitor->getDeltaTime();
			float yvel = props.getYvel() * dtMonitor->getDeltaTime();
			props.incrementXpos(xvel * collisionTime);
			props.incrementYpos(yvel * collisionTime);
			float remainingTime = 1.0f - collisionTime;
			float dotprod = (xvel * normaly + yvel * normalx) * remainingTime;
			props.setXvel((dotprod * normaly) / dtMonitor->getDeltaTime());
			props.setYvel((dotprod * normalx) / dtMonitor->getDeltaTime());
		}
	}

	bool CollisionDetector::AABB(const EntityShape& b1, const EntityShape& b2) const
	{
		 return !(b1.x > b2.x + b2.w)
			 && !(b1.x + b1.w < b2.x)
			 && !(b1.y > b2.y + b2.h)
			 && !(b1.y + b1.h < b2.y);
	}

	float CollisionDetector::sweptAABB(EntityProperties* p1, EntityProperties* p2,
			float *normalx, float *normaly) const
	{
		EntityShape b1 = p1->getColliderShape();
		EntityShape b2 = p2->getColliderShape();

		float xvel = p1->getXvel() * dtMonitor->getDeltaTime();
		float yvel = p1->getYvel() * dtMonitor->getDeltaTime();


		// Find the distances to the blocking object
		float xInvEntry, xInvExit;
		float yInvEntry, yInvExit;

		if (xvel > 0.0f) {
			xInvEntry = b2.x - (b1.x + b1.w + 2);
			xInvExit = (b2.x + b2.w) - (b1.x - 2);
		} else {
			xInvEntry = (b2.x + b2.w) - (b1.x - 2);
			xInvExit = b2.x - (b1.x + b1.w + 2);
		}

		if (yvel > 0.0f) {
			yInvEntry = b2.y - (b1.y + b1.w + 2);
			yInvExit = (b2.y + b2.w) - (b1.y - 2);
		} else {
			yInvEntry = (b2.y + b2.w) - (b1.y - 2);
			yInvExit = b2.y - (b1.y + b1.w + 2);
		}

		// Find the impact time
		float xEntry, xExit;
		float yEntry, yExit;

		if (xvel == 0.0f) {
			xEntry = -std::numeric_limits<float>::infinity();
			xExit = std::numeric_limits<float>::infinity();
		} else {
			xEntry = xInvEntry / xvel;
			xExit = xInvExit / xvel;
		}

		if (yvel == 0.0f) {
			yEntry = -std::numeric_limits<float>::infinity();
			yExit = std::numeric_limits<float>::infinity();
		} else {
			yEntry = yInvEntry / yvel;
			yExit = yInvExit / yvel;
		}

		// Find earliest/latest times of collision
		float entryTime = std::max(xEntry, yEntry);
		float exitTime = std::min(xExit, yExit);

		if (entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f) || xEntry > 1.0f || yEntry > 1.0f ) {
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
