#ifndef CUSTOMGAMEDATA_H_
#define CUSTOMGAMEDATA_H_

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
			: resourceLoader(rl), resourceContainer(rc), particleEngine(pe), layerService(ls) { }

	public:
		~CustomGameData() {
			delete resourceLoader;
			delete resourceContainer;
			delete particleEngine;
			delete layerService;
		}

		static CustomGameData* create(flat2d::GameData *gd) {
			LayerService *layerService = new LayerService();
			return new CustomGameData(
					new ResourceLoader(),
					new ResourceContainer(),
					new ParticleEngine(gd->getEntityContainer(), layerService),
					layerService
					);
		}

		ResourceContainer* getResourceContainer() { return resourceContainer; }
		ResourceLoader* getResourceLoader() { return resourceLoader; }
		ParticleEngine* getParticleEngine() { return particleEngine; }
		LayerService* getLayerService() { return layerService; }
};

#endif // CUSTOMGAMEDATA_H_
