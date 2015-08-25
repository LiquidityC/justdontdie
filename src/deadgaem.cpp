#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <flat/flat.h>
#include <iostream>

#include "Soldier.h"
#include "ParticleEngine.h"
#include "ResourceContainer.h"
#include "GameSettings.h"
#include "MapParser.h"
#include "LayerService.h"
#include "ResourceLoader.h"
#include "CustomGameData.h"
#include "FrameCounter.h"

int main( int argc, char* args[] )
{
	flat2d::FlatBuilder *flat = new flat2d::FlatBuilder;
	if (!flat->initSDL("DeadGaem", GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT)) {
		return -1;
	}
	if (!flat->initContainers()) {
		return -1;
	}

	// TODO(Linus): Should probably extract all this into a function/method {{
	flat2d::GameData *gameData = flat->getGameData();
	flat2d::EntityContainer* entityContainer = gameData->getEntityContainer();
	entityContainer->setSpatialPartitionDimension(100);

	CustomGameData *customGameData = CustomGameData::create(gameData);
	gameData->setCustomGameData(customGameData);

	customGameData->getLayerService()->registerLayers(entityContainer);

	MapParser parser;
	parser.createMapFrom(gameData, "resources/maps/map2/", "map2.tmx");

	flat2d::Entity* soldier = new Soldier(200, 200);
	soldier->init(gameData);
	entityContainer->registerObject(soldier, Layers::MID);

	ResourceLoader *rLoader = customGameData->getResourceLoader();
	rLoader->loadMusic(gameData);
	rLoader->loadSoundEffects(gameData);

#ifdef DEBUG
	FrameCounter *counter = new FrameCounter(10, 10);
	counter->init(gameData);
	entityContainer->registerObject(counter, Layers::OVERLAY);
#endif // DEBUG

	// }}

	flat2d::GameEngine *engine = flat->getGameEngine();
	engine->init(GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT, GameSettings::SCREEN_FPS);

	// Start the game loop
	engine->run();

	delete customGameData;
	delete flat;

	return 0;
}
