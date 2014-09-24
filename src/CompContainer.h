#ifndef _COMP_CONTAINER_H
#define _COMP_CONTAINER_H

#include <flat/ObjectContainer.h>
#include <flat/CollisionDetector.h>

#include "PathGenerator.h"

class CompContainer
{
	private:
		flat2d::ObjectContainer objectContainer;
		flat2d::CollisionDetector collisionDetector;
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

		flat2d::ObjectContainer& getObjectContainer() { return objectContainer; };
		flat2d::CollisionDetector& getCollisionDetector() { return collisionDetector; };
		PathGenerator& getPathGenerator() { return pathGenerator; };
};

#endif
