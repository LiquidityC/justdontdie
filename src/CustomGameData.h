#ifndef _CUSTOM_GAME_DATA_H
#define _CUSTOM_GAME_DATA_H

#include <flat/flat.h>
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

		CustomGameData(ResourceLoader *rl, ResourceContainer *rc, ParticleEngine *pe, LayerService *ls)
			: resourceLoader(rl), resourceContainer(rc), particleEngine(pe), layerService(ls) { };

	public:
		~CustomGameData() {
			delete resourceLoader;
			delete resourceContainer;
			delete particleEngine;
			delete layerService;
		}

		static CustomGameData* create(flat2d::GameData *gd) {
			return new CustomGameData(
					new ResourceLoader(),
					new ResourceContainer(),
					new ParticleEngine(gd->getEntityContainer()),
					new LayerService()
					);
		}

		ResourceContainer* getResourceContainer() { return resourceContainer; };
		ResourceLoader* getResourceLoader() { return resourceLoader; };
		ParticleEngine* getParticleEngine() { return particleEngine; };
		LayerService* getLayerService() { return layerService; };
};

#endif
