#ifndef _PARTICLE_ENGINE_H
#define _PARTICLE_ENGINE_H

#include <SDL2/SDL.h>
#include <flat/flat.h>

class ParticleEngine
{
	flat2d::EntityContainer *entityContainer;

	public:
		ParticleEngine(flat2d::EntityContainer *obc) : entityContainer(obc) { };

		void createBloodSprayAt(int, int);

		void createGhostSprayAt(int, int);

		void createExplosionAt(int, int);
};

#endif
