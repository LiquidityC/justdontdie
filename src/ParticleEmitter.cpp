#include <iostream>
#include "ParticleEmitter.h"
#include "Particle.h"
#include "BloodParticle.h"
#include "GhostParticle.h"
#include "FireParticle.h"
#include "BoostParticle.h"
#include "LayerService.h"
#include "CustomGameData.h"
#include "Random.h"

void ParticleEmitter::emit(const flat2d::GameData *gameData, SDL_Rect area)
{
	if (!running && emitCount <= 0) {
		return;
	}

	size_t count =  20;
	if (emitCount > count) {
		emitCount -= count;
	} else if (emitCount > 0) {
		count = emitCount;
		emitCount = 0;
	}

	flat2d::EntityContainer *entityContainer = gameData->getEntityContainer();
	LayerService *layerService = static_cast<CustomGameData*>(gameData->getCustomGameData())->getLayerService();
	int layerIndex = layerService->getLayerIndex(FRONT_LAYER);
	for (size_t i = 0; i < count; i++) {
		int x = getRandomBetween(area.x, area.x + area.w);
		int y = getRandomBetween(area.y, area.y + area.h);

		Particle *particle = createParticleAt(x, y);
		if (particle != nullptr) {
			entityContainer->registerObject(particle, layerIndex);
		}
	}
}

Particle* ParticleEmitter::createParticleAt(int x, int y)
{
	Particle *particle = nullptr;
	switch (type) {
		case BLOOD_PARTICLE:
			particle = new BloodParticle(x, y, 3, getRandomBetween(-500, 500), getRandomBetween(-500, 500));
			break;
		case GHOST_PARTICLE:
			particle = new GhostParticle(x, y, 2, 2, getRandomBetween(-500, 500), getRandomBetween(-300, 300));
			break;
		case FIRE_PARTICLE:
			particle = new FireParticle(x, y, getRandomBetween(-300, 300), getRandomBetween(-300, 300));
			break;
		case BOOST_PARTICLE:
			particle = new BoostParticle(x, y, getRandomBetween(-300, 300), getRandomBetween(-300, 300));
			break;
		default:
			break;
	}
	return particle;
}

void ParticleEmitter::setEmissionCount(size_t count)
{
	this->emitCount = count;
}

void ParticleEmitter::setRunning(bool running)
{
	this->running = running;
}
