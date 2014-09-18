#ifndef _COMP_CONTAINER_H
#define _COMP_CONTAINER_H

#include "objectcontainer.h"
#include "CollisionDetector.h"

class CompContainer
{
	private:
		ObjectContainer objectContainer;
		CollisionDetector collisionDetector;

	private:

		// Don't implement, singleton
		CompContainer() : 
			objectContainer(), 
			collisionDetector(&objectContainer) { };

		CompContainer(const CompContainer&);
		CompContainer& operator=(const CompContainer&);

	public:

		static CompContainer& getInstance()
		{
			static CompContainer instance;
			return instance;
		}

		ObjectContainer& getObjectContainer() { return objectContainer; };
		CollisionDetector& getCollisionDetector() { return collisionDetector; };
};

#endif
