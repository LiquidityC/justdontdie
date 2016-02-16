#include "AngryBot.h"

void AngryBot::init(const flat2d::GameData *gameData)
{
	Enemy::init(gameData);

	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/angry_bot.png",
				gameData->getRenderData()->getRenderer()));
	SDL_Rect clip = { 0, 0, 32, 32 };
	setClip(clip);

	flat2d::EntityShape shape = { 4, 9, 24, 23 };
	entityProperties.setColliderShape(shape);
}

void AngryBot::preMove(const flat2d::GameData *gameData)
{
	SDL_Rect clip = { 0, 0, 32, 32 };
	if (entityProperties.getXvel() > 0) {
		clip = { 32, 0, 32, 32 };
	}
	setClip(clip);
	Enemy::preMove(gameData);
}
