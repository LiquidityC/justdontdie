#ifndef _COMP_CONTAINER_H
#define _COMP_CONTAINER_H

#include <flat/flat.h>

#include "ResourceContainer.h"
#include "GameSettings.h"
#include "Layers.h"
#include "ParticleEngine.h"

class CompContainer
{
	private:
		flat2d::ObjectContainer objectContainer;
		flat2d::CollisionDetector collisionDetector;
		flat2d::Camera camera;
		ResourceContainer resourceContainer;
		ParticleEngine particleEngine;

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
		flat2d::Camera& getCamera() { return camera; }
		ResourceContainer& getResourceContainer() { return resourceContainer; }
		ParticleEngine& getParticleEngine() { return particleEngine; }
};

#endif
