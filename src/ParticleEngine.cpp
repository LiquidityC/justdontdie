#include <iostream>
#include "ParticleEngine.h"
#include "BloodParticle.h"
#include "Layers.h"

void ParticleEngine::createBloodSprayAt(int x, int y)
{
	Particle *p;
	for (auto i = 0; i < 500; i++) {
		int xneg = rand() % 2 > 0 ? 1 : -1;
		int yneg = rand() % 2 > 0 ? 1 : -1;
		int xvel = (rand() % 1000) * xneg;
		int yvel = (rand() % 1000) * yneg;
		p = new BloodParticle(x, y, xvel, yvel);
		objectContainer->registerObject(p, Layers::FRONT);
	}
}
