#ifndef GHOSTPARTICLE_H_
#define GHOSTPARTICLE_H_

#include <flat/flat.h>
#include "Particle.h"

class GhostParticle : public Particle
{
	public:
		GhostParticle(int x, int y, int w, int h, int xvel, int yvel) :
			Particle(x, y, w, h, xvel, yvel) {
				entityProperties.setCollisionProperty(flat2d::CollisionProperty::BOUNCY);
			}

		void preMove(const flat2d::GameData*);

	private:
		void setRenderDrawColor(SDL_Renderer* renderer) const;
};

#endif // GHOSTPARTICLE_H_
