#ifndef _COMP_CONTAINER_H
#define _COMP_CONTAINER_H

#include <flat-2d/ObjectContainer.h>
#include <flat-2d/CollisionDetector.h>

#include "PathGenerator.h"

class CompContainer
{
	private:
		ObjectContainer objectContainer;
		CollisionDetector collisionDetector;
		PathGenerator pathGenerator;

	private:

		// Don't implement, singleton
		CompContainer() : 
			objectContainer(), 
			collisionDetector(&objectContainer),
			pathGenerator(&objectContainer) { };

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
		PathGenerator& getPathGenerator() { return pathGenerator; };
};

#endif
