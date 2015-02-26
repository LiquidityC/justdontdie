#include <algorithm>
#include "GhostParticle.h"
#include "GameObjectType.h"

void GhostParticle::preRender(const flat2d::GameData* data)
{
	float deltaTime = data->getDeltatimeMonitor()->getDeltaTime();

	if (yvel < 800) {
		yvel += std::min(3600 * deltaTime, 800 - yvel);
	}

	flat2d::CollisionDetector *colDetector = data->getCollisionDetector();

	locationProperty.incrementXpos(xvel * deltaTime);
	GameObject *object = colDetector->checkForCollisions(this);
	if (object && object->getType() != GameObjectType::ROCKET) {
		locationProperty.incrementXpos(-(xvel * deltaTime));

		reduceXVel(1800, deltaTime);
		xvel *= -1;

		object = nullptr;
	}

	locationProperty.incrementYpos(yvel * deltaTime);
	object = colDetector->checkForCollisions(this);
	if (object && object->getType() != GameObjectType::ROCKET) {
		locationProperty.incrementYpos(-(yvel * deltaTime));

		reduceYVel(3600, deltaTime);
		yvel *= -1;
	}

	reduceXVel(900, deltaTime);
	reduceYVel(1800, deltaTime);
}

void GhostParticle::setRenderDrawColor(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 0x91, 0x8c, 0xFF, 0xFF);
}
