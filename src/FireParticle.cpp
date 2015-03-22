#include "FireParticle.h"
#include "EntityType.h"

void FireParticle::preRender(const flat2d::GameData *data)
{
	if (xvel == 0 && yvel == 0) {
		setDead(true);
		return;
	}

	float deltaTime = data->getDeltatimeMonitor()->getDeltaTime();
	flat2d::CollisionDetector *colDetector = data->getCollisionDetector();
	entityProperties.incrementXpos(xvel * deltaTime);
	entityProperties.incrementYpos(yvel * deltaTime);

	Entity *object = colDetector->checkForCollisions(this);
	if (object && object->getType() != EntityType::ROCKET) {
		setDead(true);
	}

	reduceXVel(900, deltaTime);
	reduceYVel(900, deltaTime);
}

void FireParticle::setRenderDrawColor(SDL_Renderer *renderer) const
{
	static int random = rand() % 2;

	switch (random) {
		case 0:
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xa2, 0x00, 0xFF);
			break;
		case 1:
		default:
			SDL_SetRenderDrawColor(renderer, 0xfa, 0xfd, 0x00, 0xFF);
			break;
	}
}

void FireParticle::postRender(const flat2d::GameData *data)
{
	if (!deathTimer.isStarted()) {
		deathTimer.start();
	}
	if (!isDead()) {
		setDead(deathTimer.getTicks() > 500);
	}
}
