#include <algorithm>
#include "GhostParticle.h"
#include "GameObjectType.h"

void GhostParticle::preRender(const flat2d::RenderData* data)
{
	float deltaTime = data->getCamera()->getDeltaTime();

	if (yvel < 800) {
		yvel += std::min(3600 * deltaTime, 800 - yvel);
	}

	flat2d::CollisionDetector *colDetector = data->getCollisionDetector();

	xpos += (xvel * deltaTime);
	GameObject *object = colDetector->checkForCollisions(this);
	if (object && object->getType() != GameObjectType::ROCKET) {
		xpos -= (xvel * deltaTime);

		reduceXVel(1800, deltaTime);
		xvel *= -1;

		object = NULL;
	}

	ypos += (yvel * deltaTime);
	object = colDetector->checkForCollisions(this);
	if (object && object->getType() != GameObjectType::ROCKET) {
		ypos -= (yvel * deltaTime);

		reduceYVel(3600, deltaTime);
		yvel *= -1;
	}

	reduceXVel(900, deltaTime);
	reduceYVel(1800, deltaTime);
}

void GhostParticle::setRenderDrawColor(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 0x91, 0x8c, 0xFF, 0xF);
}

void GhostParticle::reduceXVel(int reduction, float deltaTime)
{
	xvel = getReducedVelocity(xvel, reduction, deltaTime);
}

void GhostParticle::reduceYVel(int reduction, float deltaTime)
{
	yvel = getReducedVelocity(yvel, reduction, deltaTime);
}

float GhostParticle::getReducedVelocity(float vel, int reduction, float deltaTime)
{
	if (vel < 50 && vel > -50) {
		return 0;
	}

	float calculatedReduction = reduction * deltaTime;
	if (std::abs(vel) < calculatedReduction) {
		return 0;
	}

	if (vel > 0) {
		return vel - calculatedReduction;
	} else {
		return vel + calculatedReduction;
	}
}
