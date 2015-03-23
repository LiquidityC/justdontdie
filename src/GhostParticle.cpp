#include <algorithm>
#include "GhostParticle.h"
#include "EntityType.h"

void GhostParticle::preRender(const flat2d::GameData* data)
{
	float deltaTime = data->getDeltatimeMonitor()->getDeltaTime();

	if (entityProperties.getYvel() < 800) {
		entityProperties.setYvel(entityProperties.getYvel() + std::min(3600 * deltaTime, 800 - entityProperties.getYvel()));
	}

	flat2d::CollisionDetector *colDetector = data->getCollisionDetector();

	entityProperties.incrementXpos(entityProperties.getXvel() * deltaTime);
	Entity *object = colDetector->checkForCollisions(this);
	if (object && object->getType() != EntityType::ROCKET) {
		entityProperties.incrementXpos(-(entityProperties.getXvel() * deltaTime));

		reduceXVel(1800 + entityProperties.getWidth() * 250, deltaTime);
		entityProperties.setXvel(entityProperties.getXvel() * -1);

		object = nullptr;
	}

	entityProperties.incrementYpos(entityProperties.getYvel() * deltaTime);
	object = colDetector->checkForCollisions(this);
	if (object && object->getType() != EntityType::ROCKET) {
		entityProperties.incrementYpos(-(entityProperties.getYvel() * deltaTime));

		reduceYVel(4600 + entityProperties.getHeight() * 1000, deltaTime);
		entityProperties.setYvel(entityProperties.getYvel() * -1);
	}

	reduceXVel(600, deltaTime);
	reduceYVel(1800, deltaTime);
}

void GhostParticle::setRenderDrawColor(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 0x91, 0x8c, 0xFF, 0xFF);
}
