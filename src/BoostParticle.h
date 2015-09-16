#ifndef BOOSTPARTICLE_H_
#define BOOSTPARTICLE_H_

#include "Particle.h"

class BoostParticle : public Particle
{
	public:
		BoostParticle(int x, int y) : Particle(x, y, 2, 2, 0, 0) {
			entityProperties.setCollisionProperty(flat2d::CollisionProperty::ETHERAL);
		}

	private:
		void setRenderDrawColor(SDL_Renderer *renderer) const;
		void preMove(const flat2d::GameData*) { }
		void postRender(const flat2d::GameData*);
}; // class BoostParticle

#endif // BOOSTPARTICLE_H_
