#ifndef _COMP_CONTAINER_H
#define _COMP_CONTAINER_H

#include <flat/ObjectContainer.h>
#include <flat/CollisionDetector.h>
#include <flat/Camera.h>

#include "ResourceContainer.h"
#include "GameSettings.h"
#include "Layers.h"

class CompContainer
{
	private:
		flat2d::ObjectContainer objectContainer;
		flat2d::CollisionDetector collisionDetector;
		Camera camera;
		ResourceContainer resourceContainer;

	private:

		// Don't implement, singleton
		CompContainer() : 
			objectContainer(), 
			collisionDetector(&objectContainer),
			camera(0, 0, GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT),
			resourceContainer() { 
				objectContainer.addLayer(Layers::BACK);
				objectContainer.addLayer(Layers::MID);
				objectContainer.addLayer(Layers::FRONT);
			};

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
		Camera& getCamera() { return camera; }
		ResourceContainer& getResourceContainer() { return resourceContainer; }
};

#endif
