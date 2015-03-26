#include <algorithm>
#include "GhostParticle.h"
#include "EntityType.h"

void GhostParticle::preRender(const flat2d::GameData* data)
{
	float deltaTime = data->getDeltatimeMonitor()->getDeltaTime();

	if (entityProperties.getYvel() < 800) {
		entityProperties.setYvel(entityProperties.getYvel() + 60);
	}

	reduceXVel(600, deltaTime);
	reduceYVel(1800, deltaTime);
}

void GhostParticle::setRenderDrawColor(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 0x91, 0x8c, 0xFF, 0xFF);
}
