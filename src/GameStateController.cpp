#include <iostream>
#include "GameStateController.h"
#include "MapParser.h"
#include "CustomGameData.h"
#include "Soldier.h"
#include "npcs/Enemy.h"
#include "npcs/ai/WalkingAgent.h"
#include "FrameCounter.h"

GameStateController::GameStateController()
{
	initMaps();
}

void GameStateController::handle(const SDL_Event& event)
{
#ifndef DBUG
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_5) {
			reset = true;
		}
	}
#endif
}

bool GameStateController::gameStateCheck(flat2d::GameData *gameData)
{
	if (reset) {
		resetGame(gameData);
		reset = false;
		return true;
	}
	return false;
}

void GameStateController::initMaps()
{
	maps.clear();
	maps.push_back({ "resources/maps/world1/", "map1.tmx" });
	maps.push_back({ "resources/maps/world1/", "map2.tmx" });
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
	customGameData->getResourceContainer()->clearTextures();

	// Make sure layers are cleared before parsing the map
	LayerService *layerService = customGameData->getLayerService();
	layerService->clearAllLayers();

	MapParser parser;
	MapData map = maps[currentMapIndex];
	parser.createMapFrom(gameData, map.path, map.file);

	// TODO(Linus): Will this layer system work? (esp the hardcoded layers)
	layerService->registerLayer(FRONT_LAYER);
	layerService->registerLayer(OVERLAY_LAYER);
	layerService->registerLayers(entityContainer);

	flat2d::Entity* soldier = new Soldier(200, 200);
	soldier->init(gameData);

	Agent *agent = new WalkingAgent(-200);
	flat2d::Entity *enemy = new Enemy(agent, 300, 200, 15, 15);
	entityContainer->registerObject(enemy, layerService->getLayerIndex(FRONT_LAYER));

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

void GameStateController::loadNextMap()
{
	if (currentMapIndex + 1 >= maps.size()) {
		return;
	}

	currentMapIndex++;
	reset = true;
}
