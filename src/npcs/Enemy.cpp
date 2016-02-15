#include "Enemy.h"
#include "../MoveUtil.h"

void Enemy::init(const flat2d::GameData *gameData)
{
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/angry_bot.png",
				gameData->getRenderData()->getRenderer()));
	SDL_Rect clip = { 0, 0, 32, 32 };
	setClip(clip);

	entityProperties.setCollidable(true);
}

bool Enemy::onCollision(flat2d::Entity *collider, const flat2d::GameData *data)
{
	if (collider->getType() != EntityType::TILE) {
		return true;
	}
	return aiAgent->onCollision(collider, this, data);
}

bool Enemy::onVerticalCollision(flat2d::Entity *collider, const flat2d::GameData *data)
{
	if (collider->getType() != EntityType::TILE) {
		return true;
	}
	return aiAgent->onVerticalCollision(collider, this, data);
}

bool Enemy::onHorizontalCollision(flat2d::Entity *collider, const flat2d::GameData *data)
{
	if (collider->getType() != EntityType::TILE && collider->getType() != EntityType::ENEMY) {
		return true;
	}
	return aiAgent->onHorizontalCollision(collider, this, data);
}

void Enemy::preMove(const flat2d::GameData* data)
{
	MoveUtil::applyGravity(&entityProperties);
	aiAgent->handleEntity(this);
}
