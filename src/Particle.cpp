#include <iostream>
#include <algorithm>
#include "Particle.h"
#include "EntityType.h"

void Particle::preRender(const flat2d::GameData* data)
{
	float deltaTime = data->getDeltatimeMonitor()->getDeltaTime();
	if (entityProperties.getXvel() == 0 && entityProperties.getYvel() == 0) {
		return;
	}

	if (entityProperties.getYvel() < 800) {
		entityProperties.setYvel(entityProperties.getYvel() + std::min(3600 * deltaTime, 800 - entityProperties.getYvel()));
	}

	flat2d::CollisionDetector *colDetector = data->getCollisionDetector();

	entityProperties.incrementXpos(entityProperties.getXvel() * deltaTime);
	entityProperties.incrementYpos(entityProperties.getYvel() * deltaTime);

	Entity *object = colDetector->checkForCollisions(this);
	if (object && object->getType() != EntityType::ROCKET) {
		entityProperties.setXvel(0);
		entityProperties.setYvel(0);
	}
}

void Particle::render(const flat2d::RenderData* data) const
{
	flat2d::Camera *cam = data->getCamera();

	SDL_Renderer *renderer = data->getRenderer();
	setRenderDrawColor(renderer);
	SDL_Rect box = { cam->getScreenXposFor(entityProperties.getXpos()),
		cam->getScreenYposFor(entityProperties.getYpos()),
		entityProperties.getWidth(), entityProperties.getHeight() };

	SDL_SetRenderDrawBlendMode(renderer, blendMode);
	SDL_RenderFillRect( renderer, &box );
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Particle::postRender(const flat2d::GameData *data)
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
	entityProperties.setXvel(getReducedVelocity(entityProperties.getXvel(), reduction, deltaTime));
}

void Particle::reduceYVel(int reduction, float deltaTime)
{
	entityProperties.setYvel(getReducedVelocity(entityProperties.getYvel(), reduction, deltaTime));
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
