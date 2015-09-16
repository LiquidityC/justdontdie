#include <random>
#include "BoostParticle.h"

void BoostParticle::setRenderDrawColor(SDL_Renderer *renderer) const
{
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_int_distribution<> dis(0, 1);

	switch (dis(gen)) {
		case 0:
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			break;
		case 1:
		default:
			SDL_SetRenderDrawColor(renderer, 0xfa, 0xfd, 0x00, 0xFF);
			break;
	}
}

void BoostParticle::postRender(const flat2d::GameData *data)
{
	if (!deathTimer.isStarted()) {
		deathTimer.start();
	}
	if (!isDead()) {
		setDead(deathTimer.getTicks() > 100);
	}
}
