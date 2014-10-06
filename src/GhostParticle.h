#ifndef _GHOST_PARTICLE_H
#define _GHOST_PARTICLE_H

#include <flat/flat.h>
#include "Particle.h"

class GhostParticle : public Particle
{
	public:
		GhostParticle(int x, int y, int w, int h, int xvel, int yvel) :
			Particle(x, y, w, h, xvel, yvel) { };

		void preRender(const flat2d::RenderData*);

	private:
		void setRenderDrawColor(SDL_Renderer* renderer) const;
};

#endif
