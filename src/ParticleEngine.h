#ifndef PARTICLEENGINE_H_
#define PARTICLEENGINE_H_

#include <SDL2/SDL.h>
#include <flat/flat.h>

class ParticleEngine
{
	flat2d::EntityContainer *entityContainer;

	public:
		explicit ParticleEngine(flat2d::EntityContainer *obc) : entityContainer(obc) { }

		void createBloodSprayAt(int, int);

		void createGhostSprayAt(int, int);

		void createExplosionAt(int, int);
};

#endif // PARTICLEENGINE_H_
