#include "FireParticle.h"
#include "GameObjectType.h"

void FireParticle::preRender(const flat2d::RenderData *data)
{
	if (xvel == 0 && yvel == 0) {
		setDead(true);
		return;
	}

	float deltaTime = data->getCamera()->getDeltaTime();
	flat2d::CollisionDetector *colDetector = data->getCollisionDetector();
	xpos += (xvel * deltaTime);
	ypos += (yvel * deltaTime);

	GameObject *object = colDetector->checkForCollisions(this);
	if (object && object->getType() != GameObjectType::ROCKET) {
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

void FireParticle::postRender(const flat2d::RenderData *data)
{
	if (!deathTimer.isStarted()) {
		deathTimer.start();
	}
	if (!isDead()) {
		setDead(deathTimer.getTicks() > 500);
	}
}
