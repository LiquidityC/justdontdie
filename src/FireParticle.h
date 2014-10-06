#ifndef _FIRE_PARTICLE_H
#define _FIRE_PARTICLE_H

#include <flat/flat.h>
#include "Particle.h"

class FireParticle : public Particle
{
	public:
		FireParticle(int x, int y, int xv, int yv) :
			Particle(x, y, 2, 2, xv, yv) { };

		void preRender(const flat2d::RenderData*);
		void postRender(const flat2d::RenderData*);

	private:
		void setRenderDrawColor(SDL_Renderer* renderer) const;
};

#endif
