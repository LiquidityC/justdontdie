#include <iostream>
#include "GameStateController.h"
#include "map/MapParser.h"
#include "CustomGameData.h"
#include "Soldier.h"
#include "npcs/AngryBot.h"
#include "npcs/ai/WalkingAgent.h"
#include "FrameCounter.h"
#include "SplashScreen.h"

GameStateController::GameStateController()
{
	initMaps();
}

void GameStateController::handle(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN) {
#ifdef DEBUG
		if (event.key.keysym.sym == SDLK_5) {
			reset = true;
		}
#endif
		if (currentState == SPLASH && splashTimer.isStarted()) {
			splashTimer.stop();
			currentState = GAME;
			reset = true;
		}
	}
}

bool GameStateController::gameStateCheck(flat2d::GameData *gameData)
{
	if (splashTimer.isStarted() && splashTimer.getTicks() > 5000) {
		splashTimer.stop();
		currentState = GAME;
		reset = true;
	}

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
	switch (currentState) {
		case SPLASH:
			loadSplash(gameData);
			break;
		case GAME:
		default:
			loadGame(gameData);
			break;
	}
}

void GameStateController::clearAllAssets(flat2d::GameData *gameData)
{
	flat2d::EntityContainer* entityContainer = gameData->getEntityContainer();

	// Clear possible existing data
	entityContainer->unregisterAllObjects();
	gameData->getMixer()->clearAllSound();

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
}

void GameStateController::loadSplash(flat2d::GameData *gameData)
{
	// Clean up first
	clearAllAssets(gameData);

	flat2d::Texture *texture = new flat2d::Texture(0, 0);
	texture->loadFromFile("resources/images/oliveshark_logo.png", gameData->getRenderData()->getRenderer());

	SplashScreen *splash = new SplashScreen("resources/images/oliveshark_logo.png");
	gameData->getEntityContainer()->registerObject(splash);

	splashTimer.start();
}

void GameStateController::loadGame(flat2d::GameData *gameData)
{
	// Clean up first
	clearAllAssets(gameData);

	flat2d::EntityContainer *entityContainer = gameData->getEntityContainer();
	CustomGameData *customGameData = static_cast<CustomGameData*>(gameData->getCustomGameData());
	LayerService *layerService = customGameData->getLayerService();

	MapParser parser;
	MapData map = maps[currentMapIndex];
	parser.createMapFrom(gameData, map.path, map.file);

	// TODO(Linus): Will this layer system work? (esp the hardcoded layers)
	layerService->registerLayer(FRONT_LAYER);
	layerService->registerLayer(OVERLAY_LAYER);
	layerService->registerLayers(entityContainer);

	flat2d::Entity* soldier = new Soldier(200, 200);
	entityContainer->registerObject(soldier, layerService->getLayerIndex(FRONT_LAYER));

	ResourceLoader *rLoader = customGameData->getResourceLoader();
	rLoader->loadMusic(gameData);
	rLoader->loadSoundEffects(gameData);

#ifdef FPS_DBG
	FrameCounter *counter = new FrameCounter(10, 10);
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

bool GameStateController::quit()
{
	return false;
}
