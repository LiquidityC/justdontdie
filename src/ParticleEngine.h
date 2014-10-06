#ifndef _PARTICLE_ENGINE_H
#define _PARTICLE_ENGINE_H

#include <SDL2/SDL.h>
#include <flat/flat.h>

class ParticleEngine
{
	flat2d::ObjectContainer *objectContainer;

	public:
		ParticleEngine(flat2d::ObjectContainer *obc) : objectContainer(obc) { };

		void createBloodSprayAt(int, int);

		void createGhostSprayAt(int, int);

		void createExplosionAt(int, int);
};

#endif
