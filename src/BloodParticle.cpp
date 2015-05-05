#include <iostream>
#include <algorithm>
#include "BloodParticle.h"
#include "EntityType.h"

void BloodParticle::setRenderDrawColor(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
}

bool BloodParticle::onCollision(flat2d::Entity *collider, const flat2d::GameData *data)
{
	if (Particle::onCollision(collider, data)) {
		return true;
	}

	if (collider->getType() == EntityType::SOLDIER) {
		return true;
	}

	entityProperties.move(data->getDeltatimeMonitor()->getDeltaTime());
	entityProperties.setXvel(0);
	entityProperties.setYvel(0);

	return true;
}
