#ifndef BOOSTPARTICLE_H_
#define BOOSTPARTICLE_H_

#include "Particle.h"

class BoostParticle : public Particle
{
	public:
		BoostParticle(int x, int y, int xvel, int yvel) : Particle(x, y, 2, 2, xvel, yvel) {
			entityProperties.setCollisionProperty(flat2d::CollisionProperty::ETHERAL);
		}

	private:
		void setRenderDrawColor(SDL_Renderer *renderer) const;
		void preMove(const flat2d::GameData*) { }
		void postRender(const flat2d::GameData*);
}; // class BoostParticle

#endif // BOOSTPARTICLE_H_
