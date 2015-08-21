#ifndef BLOODPARTICLE_H_
#define BLOODPARTICLE_H_

#include <flat/flat.h>
#include <random>
#include "Particle.h"

class BloodParticle : public Particle
{
	private:
		flat2d::Timer timer;

	public:
		BloodParticle(int x, int y, int size, int xvel, int yvel)
			: Particle(x, y, size, size, xvel, yvel) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(0, 1);
				if ( dis(gen) == 0 ) {
					setBlendMode(SDL_BLENDMODE_NONE);
				}
				entityProperties.setCollisionProperty(flat2d::CollisionProperty::STICKY);
			}

	private:
		void setRenderDrawColor(SDL_Renderer* renderer) const;
};

#endif // BLOODPARTICLE_H_
