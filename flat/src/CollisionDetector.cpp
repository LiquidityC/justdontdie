#include "GameObject.h"
#include "CollisionDetector.h"
#include "ObjectContainer.h"

using namespace flat2d;

CollisionDetector::CollisionDetector(ObjectContainer* objectContainer) 
{
	this->objectContainer = objectContainer;
}

GameObject* CollisionDetector::checkForCollisions(const GameObject* o1) const
{
	return objectContainer->checkAllObjects(
			[&](const GameObject* o2)
			{
				return this->hasCollided(o1, o2);
			});
}

bool CollisionDetector::hasCollided(const GameObject* o1, const GameObject* o2) const
{
	if (*o1 == *o2) {
		return false;
	}

	if (!o1->isCollider() || !o2->isCollider()) {
		return false;
	}

	SDL_Rect b1 = o1->getBoundingBox();
	SDL_Rect b2 = o2->getBoundingBox();

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
