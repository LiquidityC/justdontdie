#ifndef FIREPARTICLE_H_
#define FIREPARTICLE_H_

#include <flat/flat.h>
#include "Particle.h"

class FireParticle : public Particle
{
	public:
		FireParticle(int x, int y, int xv, int yv) :
			Particle(x, y, 2, 2, xv, yv) {
				entityProperties.setCollisionProperty(flat2d::CollisionProperty::ETHERAL);
			}

		void preMove(const flat2d::GameData*);
		void postRender(const flat2d::GameData*);

	private:
		void setRenderDrawColor(SDL_Renderer* renderer) const;
};

#endif // FIREPARTICLE_H_
