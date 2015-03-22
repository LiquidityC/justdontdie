#include <iostream>
#include "ParticleEngine.h"
#include "BloodParticle.h"
#include "GhostParticle.h"
#include "FireParticle.h"
#include "LayerService.h"

void ParticleEngine::createBloodSprayAt(int x, int y)
{
	Particle *p;
	for (auto i = 0; i < 50; i++) {
		int xneg = rand() % 2 > 0 ? 1 : -1;
		int yneg = rand() % 2 > 0 ? 1 : -1;
		int xvel = (rand() % 500) * xneg;
		int yvel = (rand() % 500) * yneg;
		int size = (rand() % 3) + 1;
		p = new BloodParticle(x, y, size, xvel, yvel);
		entityContainer->registerObject(p, Layers::FRONT);
	}
}

void ParticleEngine::createGhostSprayAt(int x, int y)
{
	Particle *p;
	for (auto i = 0; i < 50; i++) {
		int xneg = rand() % 2 > 0 ? 1 : -1;
		int yneg = rand() % 2 > 0 ? 1 : -1;
		int xvel = (rand() % 500) * xneg;
		int yvel = (rand() % 500) * yneg;
		int size = (rand() % 3) + 1;
		p = new GhostParticle(x, y, size, size, xvel, yvel);
		entityContainer->registerObject(p, Layers::FRONT);
	}
}

void ParticleEngine::createExplosionAt(int x, int y)
{
	Particle *p;
	for (auto i = 0; i < 50; i++) {
		int xneg = rand() % 2 > 0 ? 1 : -1;
		int yneg = rand() % 2 > 0 ? 1 : -1;
		int xvel = (rand() % 300) * xneg;
		int yvel = (rand() % 300) * yneg;
		p = new FireParticle(x, y, xvel, yvel);
		entityContainer->registerObject(p, Layers::FRONT);
	}
}
