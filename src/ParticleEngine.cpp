#include <iostream>
#include "ParticleEngine.h"
#include "BloodParticle.h"
#include "CompContainer.h"

void ParticleEngine::createBloodSprayAt(int x, int y)
{
	flat2d::ObjectContainer& objectContainer = CompContainer::getInstance().getObjectContainer();
	flat2d::Camera& camera = CompContainer::getInstance().getCamera();

	Particle *p;
	for (auto i = 0; i < 100; i++) {
		int neg = rand() % 2 > 0 ? 1 : -1;
		int xvel = 100 + (rand() % 1000) * neg;
		int yvel = 100 + (rand() % 1000) * neg;
		p = new BloodParticle(x, y, xvel, yvel);
		p->init(NULL, &camera);
		objectContainer.registerObject(p, Layers::FRONT);
	}
}
