#include <iostream>
#include <algorithm>
#include "BloodParticle.h"

void BloodParticle::setRenderDrawColor(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
}
