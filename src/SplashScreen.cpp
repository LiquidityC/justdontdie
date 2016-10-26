#include "SplashScreen.h"

void SplashScreen::init(const flat2d::GameData *gameData)
{
	SDL_Renderer *renderer = gameData->getRenderData()->getRenderer();
	flat2d::Texture *texture = new flat2d::Texture();
	texture->loadFromFile(filename, renderer);
	setTexture(texture);
}
