#include <iostream>
#include <algorithm>
#include "Particle.h"
#include "GameObjectType.h"

void Particle::preRender(const flat2d::RenderData* data)
{
	float deltaTime = data->getCamera()->getDeltaTime();
	if (xvel == 0 && yvel == 0) {
		return;
	}

	if (yvel < 800) {
		yvel += std::min(3600 * deltaTime, 800 - yvel);
	}

	flat2d::CollisionDetector *colDetector = data->getCollisionDetector();

	xpos += (xvel * deltaTime);
	ypos += (yvel * deltaTime);

	GameObject *object = colDetector->checkForCollisions(this);
	if (object && object->getType() != GameObjectType::ROCKET) {
		xvel = 0;
		yvel = 0;
	}
}

void Particle::render(const flat2d::RenderData* data) const
{
	flat2d::Camera *cam = data->getCamera();

	SDL_Renderer *renderer = data->getRenderer();
	setRenderDrawColor(renderer);
	SDL_Rect box = { cam->getScreenXposFor(xpos), cam->getScreenYposFor(ypos), width, height };

	SDL_SetRenderDrawBlendMode(renderer, blendMode);
	SDL_RenderFillRect( renderer, &box );
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Particle::postRender(const flat2d::RenderData *data)
{
	if (!deathTimer.isStarted()) {
		deathTimer.start();
	}
	if (!isDead()) {
		setDead(deathTimer.getTicks() > 3000);
	}
}

void Particle::reduceXVel(int reduction, float deltaTime)
{
	xvel = getReducedVelocity(xvel, reduction, deltaTime);
}

void Particle::reduceYVel(int reduction, float deltaTime)
{
	yvel = getReducedVelocity(yvel, reduction, deltaTime);
}

float Particle::getReducedVelocity(float vel, int reduction, float deltaTime)
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

void Particle::setBlendMode(SDL_BlendMode blendMode)
{
	this->blendMode = blendMode;
}
