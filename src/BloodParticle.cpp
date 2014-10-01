#include <iostream>
#include <algorithm>
#include "BloodParticle.h"
#include "CompContainer.h"

void BloodParticle::postRender()
{
	if (timer.getTicks() > 5000) {
		setDead(true);
	}
}

void BloodParticle::setRenderDrawColor(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
}