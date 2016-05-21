#include "SplashScreen.h"

void SplashScreen::init(const flat2d::GameData *gameData)
{
	SDL_Renderer *renderer = gameData->getRenderData()->getRenderer();
	setTexture(flat2d::MediaUtil::loadTexture(filename, renderer));
}
