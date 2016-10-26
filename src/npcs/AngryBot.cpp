#include "AngryBot.h"

void AngryBot::init(const flat2d::GameData *gameData)
{
	Enemy::init(gameData);

	flat2d::Texture *texture = new flat2d::Texture();
	texture->loadFromFile("resources/textures/angry_bot.png",
			gameData->getRenderData()->getRenderer());
	setTexture(texture);

	flat2d::EntityShape shape = { 4, 9, 24, 23 };
	entityProperties.setColliderShape(shape);
}

void AngryBot::preMove(const flat2d::GameData *gameData)
{
	Enemy::preMove(gameData);
}
