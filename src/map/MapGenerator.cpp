#include <flat/flat.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include "MapGenerator.h"
#include "MapTileObject.h"
#include "../ResourceContainer.h"
#include "../CustomGameData.h"
#include "../Random.h"

void MapGenerator::generateMap(const flat2d::GameData* gameData, std::string tilemap) const
{
	// Sanity check
	assert(standardRooms.size() > 0);

	int roomCount = standardRooms.size();

	gameData->getRenderData()->getCamera()->setMapDimensions(32*100, 32*30);

	SDL_Renderer *renderer = gameData->getRenderData()->getRenderer();
	ResourceContainer *resourceContainer = ((CustomGameData*) gameData->getCustomGameData())->getResourceContainer();
	flat2d::EntityContainer *entityContainer = gameData->getEntityContainer();

	SDL_Rect r = { 0, 0, 32, 32 };

	SDL_Texture *fgTexture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			32, 32);

	SDL_SetRenderTarget(renderer, fgTexture);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
	SDL_RenderDrawRect(renderer, &r);
	resourceContainer->addTexture(fgTexture);	

	SDL_Texture *bgTexture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			32, 32);

	SDL_SetRenderTarget(renderer, bgTexture);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);
	resourceContainer->addTexture(bgTexture);	

	SDL_SetRenderTarget(renderer, nullptr);


	for (size_t i = 0; i < 10; i++) {
		for (size_t j = 0; j < 3; j++) {
			const Room *room = &(standardRooms[getRandomBetween(0, roomCount-1)]);
			for (size_t k = 0; k < 10; k++) {
				for (size_t l = 0; l < 10; l++) {
					bool solid = (*room)[(l*10)+k] == '1';
					MapTileObject *tile = new MapTileObject(
							(i*320)+(k*32),
							(j*320)+(l*32),
							32,
							32,
							solid ? fgTexture : bgTexture
							);

					tile->getEntityProperties().setCollidable(solid);
					entityContainer->registerObject(tile);
				}
			}
		}
	}
}

void MapGenerator::loadMapData()
{
	parseRoomFiles();
}

void MapGenerator::parseRoomFiles()
{
	parseRoomFile("resources/maps/rooms/standard.dat");
}

void MapGenerator::parseRoomFile(const std::string& file)
{
	std::ifstream is(file, std::ios::in);

	char c;
	int count = 0;
	Room *room = new Room();
	while (is.get(c)) {
		room->push_back(c);
		count++;
		if (count >= 100) {
			count = 0;
			standardRooms.push_back(*room);
			delete room;
			room = new Room();
		}
	}
	delete room;

	// Sanity check
	assert(count == 0);
}
