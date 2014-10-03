#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "FlatBuilder.h"
#include "GameData.h"
#include "RenderData.h"
#include "ObjectContainer.h"
#include "CollisionDetector.h"
#include "Camera.h"
#include "Window.h"

using namespace flat2d;

FlatBuilder::~FlatBuilder()
{
	delete renderData;
	delete gameData;
	delete collisionDetector;
	delete objectContainer;
	delete window;
	delete camera;
}

bool FlatBuilder::initSDL(std::string title, int screenWidth, int screenHeight)
{
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		std::cerr << "Failed to init video: " 
			<< SDL_GetError() << std::endl;
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init( imgFlags ) & imgFlags )) {
		std::cerr << "Unable to initialize SDL_image: " << IMG_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() == -1) {
		std::cerr << "Unable to initiate SDL2_ttf: " << TTF_GetError() << std::endl;
		return false;
	}

	window = new Window(title, screenWidth, screenHeight);
	if (!window->init()) {
		return false;
	}

	camera = new Camera(screenWidth, screenHeight);
	return true;
}

bool FlatBuilder::initContainers()
{
	objectContainer = new ObjectContainer();
	collisionDetector = new CollisionDetector(objectContainer);
	gameData = new GameData(objectContainer, collisionDetector);
	renderData = new RenderData(window->getRenderer(), camera, objectContainer, collisionDetector);

	return true;
}

RenderData* FlatBuilder::getRenderData() const
{
	return renderData;
}

GameData* FlatBuilder::getGameData() const
{
	return gameData;
}
