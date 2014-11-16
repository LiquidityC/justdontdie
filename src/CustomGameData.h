#ifndef _CUSTOM_GAME_DATA_H
#define _CUSTOM_GAME_DATA_H

#include <flat/flat.h>
#include <Box2D/Box2D.h>

#include "ResourceLoader.h"
#include "ResourceContainer.h"
#include "ParticleEngine.h"
#include "LayerService.h"

class CustomGameData
{
	private:
		ResourceLoader *resourceLoader = nullptr;
		ResourceContainer *resourceContainer = nullptr;
		ParticleEngine *particleEngine = nullptr;
		LayerService *layerService = nullptr;
		b2World *b2_World = nullptr;

		CustomGameData(ResourceLoader *rl,
				ResourceContainer *rc,
				ParticleEngine *pe,
				LayerService *ls,
				b2World *b2w
				)
			:
				resourceLoader(rl),
				resourceContainer(rc),
				particleEngine(pe),
				layerService(ls),
				b2_World(b2w) { };

	public:
		~CustomGameData() {
			delete resourceLoader;
			delete resourceContainer;
			delete particleEngine;
			delete layerService;
			delete b2_World;
		}

		static CustomGameData* create(flat2d::GameData *gd) {
			b2Vec2 gravity(0, 10);
			return new CustomGameData(
					new ResourceLoader(),
					new ResourceContainer(),
					new ParticleEngine(gd->getObjectContainer()),
					new LayerService(),
					new b2World(gravity)
					);
		}

		ResourceContainer* getResourceContainer() const { return resourceContainer; };
		ResourceLoader* getResourceLoader() const { return resourceLoader; };
		ParticleEngine* getParticleEngine() const { return particleEngine; };
		LayerService* getLayerService() const { return layerService; };
		b2World* getB2World() const { return b2_World; };
};

#endif
