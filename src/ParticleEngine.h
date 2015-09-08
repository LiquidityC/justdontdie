#ifndef PARTICLEENGINE_H_
#define PARTICLEENGINE_H_

#include <SDL2/SDL.h>
#include <flat/flat.h>

class LayerService;

class ParticleEngine
{
	flat2d::EntityContainer *entityContainer;
	LayerService *layerService;

	public:
		explicit ParticleEngine(flat2d::EntityContainer *obc, LayerService *ls) : entityContainer(obc), layerService(ls) { }

		void createBloodSprayAt(int, int);

		void createGhostSprayAt(int, int);

		void createExplosionAt(int, int);
};

#endif // PARTICLEENGINE_H_
