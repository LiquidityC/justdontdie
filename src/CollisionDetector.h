#ifndef _COLLISION_DETECTOR_H
#define _COLLISION_DETECTOR_H

class ObjectContainer;
class GameObject;

class CollisionDetector
{

	private:
		ObjectContainer* objectContainer;

	public:
		CollisionDetector(ObjectContainer* objectContainer);

		bool checkForCollisions(const GameObject*) const;
		bool hasCollided(const GameObject*, const GameObject*) const;

	private:
		CollisionDetector(const CollisionDetector&); // Don't implement, single instance
		const CollisionDetector& operator=(const CollisionDetector& c); // Don't use, single instance
};

#endif
