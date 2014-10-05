#ifndef _BLOOD_PARTICLE_H
#define _BLOOD_PARTICLE_H

#include <flat/flat.h>
#include "Particle.h"

class BloodParticle : public Particle
{
	private:
		GameObject *o = NULL;
		flat2d::Timer timer;

	public:
		BloodParticle(int x, int y, int xvel, int yvel) 
			: Particle(x, y, 2, 2, xvel, yvel) { };

	private:
		void setRenderDrawColor(SDL_Renderer* renderer) const;
};

#endif
