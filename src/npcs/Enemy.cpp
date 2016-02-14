#include "Enemy.h"
#include "../MoveUtil.h"

void Enemy::render(const flat2d::RenderData *data) const
{
	flat2d::Camera *camera = data->getCamera();
	SDL_SetRenderDrawColor(data->getRenderer(), 255, 0, 0, 255);

	SDL_Rect box = entityProperties.getBoundingBox();
	box.x = camera->getScreenXposFor(box.x);
	box.y = camera->getScreenYposFor(box.y);

	SDL_RenderFillRect(data->getRenderer(), &box);
}

void Enemy::init(const flat2d::GameData *data)
{
	// TODO(Linus): Mayb we don't need this?
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
	if (collider->getType() != EntityType::TILE) {
		return true;
	}
	return aiAgent->onHorizontalCollision(collider, this, data);
}

void Enemy::preMove(const flat2d::GameData* data)
{
	MoveUtil::applyGravity(&entityProperties);
	aiAgent->handleEntity(this);
}
