#include <iostream>
#include "GameStateController.h"
#include "MapParser.h"
#include "CustomGameData.h"
#include "Soldier.h"
#include "FrameCounter.h"

void GameStateController::handle(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_5) {
			reset = true;
		}
	}
}

void GameStateController::gameStateCheck(flat2d::GameData *gameData)
{
	if (reset) {
		resetGame(gameData);
		reset = false;
	}
}

void GameStateController::resetGame(flat2d::GameData *gameData)
{
	flat2d::EntityContainer* entityContainer = gameData->getEntityContainer();

	// Clear possible existing data
	entityContainer->unregisterAllObjects();
	gameData->getMixer()->clearAllSound();

	entityContainer->setSpatialPartitionDimension(100);

	// Create custom game data if it doesn't exist
	CustomGameData *customGameData = static_cast<CustomGameData*>(gameData->getCustomGameData());
	if (!customGameData) {
		customGameData = CustomGameData::create(gameData);
		customGameData->setGameStateController(this);
		gameData->setCustomGameData(customGameData);
	}

	// Make sure layers are cleared before parsing the map
	LayerService *layerService = customGameData->getLayerService();
	layerService->clearAllLayers();

	MapParser parser;
	parser.createMapFrom(gameData, "resources/maps/map2/", "map2.tmx");

	// TODO(Linus): Will this layer system work? (esp the hardcoded layers)
	layerService->registerLayer(FRONT_LAYER);
	layerService->registerLayer(OVERLAY_LAYER);
	layerService->registerLayers(entityContainer);

	flat2d::Entity* soldier = new Soldier(200, 200);
	soldier->init(gameData);

	// TODO(Linus): Maybe have something better for the layer?
	std::cout << "SOLDIER LAYER INDEX: " << layerService->getLayerIndex(FRONT_LAYER) << std::endl;
	entityContainer->registerObject(soldier, layerService->getLayerIndex(FRONT_LAYER));

	ResourceLoader *rLoader = customGameData->getResourceLoader();
	rLoader->loadMusic(gameData);
	rLoader->loadSoundEffects(gameData);

#ifdef DEBUG
	FrameCounter *counter = new FrameCounter(10, 10);
	counter->init(gameData);
	entityContainer->registerObject(counter, layerService->getLayerIndex(OVERLAY_LAYER));
#endif // DEBUG
}
