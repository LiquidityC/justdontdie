#ifndef BLOODPARTICLE_H_
#define BLOODPARTICLE_H_

#include <flat/flat.h>
#include "Particle.h"

class BloodParticle : public Particle
{
	private:
		flat2d::Timer timer;

	public:
		BloodParticle(int x, int y, int size, int xvel, int yvel)
			: Particle(x, y, size, size, xvel, yvel) {
				unsigned int seed = 598430;
				if ( (rand_r(&seed) % 2) == 0 ) {
					setBlendMode(SDL_BLENDMODE_NONE);
				}
			}

		bool onCollision(flat2d::Entity *collider, const flat2d::GameData*);

	private:
		void setRenderDrawColor(SDL_Renderer* renderer) const;
};

#endif // BLOODPARTICLE_H_
