#include <iostream>
#include "ParticleEngine.h"
#include "BloodParticle.h"
#include "GhostParticle.h"
#include "FireParticle.h"
#include "LayerService.h"

void ParticleEngine::createBloodSprayAt(int x, int y)
{
	Particle *p;
	unsigned int seed = 313213;

	for (auto i = 0; i < 150; i++) {
		int xneg = rand_r(&seed) % 2 > 0 ? 1 : -1;
		int yneg = rand_r(&seed) % 2 > 0 ? 1 : -1;
		int xvel = (rand_r(&seed) % 200) * xneg;
		int yvel = (rand_r(&seed) % 200) * yneg;
		int size = (rand_r(&seed) % 3) + 1;
		p = new BloodParticle(x, y, size, xvel, yvel);
		entityContainer->registerObject(p, layerService->getLayerIndex(FRONT_LAYER));
	}
}

void ParticleEngine::createGhostSprayAt(int x, int y)
{
	Particle *p;
	unsigned int seed = 748329;

	for (auto i = 0; i < 50; i++) {
		int xneg = rand_r(&seed) % 2 > 0 ? 1 : -1;
		int yneg = rand_r(&seed) % 2 > 0 ? 1 : -1;
		int xvel = (rand_r(&seed) % 500) * xneg;
		int yvel = (rand_r(&seed) % 500) * yneg;
		int size = (rand_r(&seed) % 3) + 1;
		p = new GhostParticle(x, y, size, size, xvel, yvel);
		entityContainer->registerObject(p, layerService->getLayerIndex(FRONT_LAYER));
	}
}

void ParticleEngine::createExplosionAt(int x, int y)
{
	Particle *p;
	unsigned int seed = 327681;

	for (auto i = 0; i < 50; i++) {
		int xneg = rand_r(&seed) % 2 > 0 ? 1 : -1;
		int yneg = rand_r(&seed) % 2 > 0 ? 1 : -1;
		int xvel = (rand_r(&seed) % 300) * xneg;
		int yvel = (rand_r(&seed) % 300) * yneg;
		p = new FireParticle(x, y, xvel, yvel);
		entityContainer->registerObject(p, layerService->getLayerIndex(FRONT_LAYER));
	}
}
