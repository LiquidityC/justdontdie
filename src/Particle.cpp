#include <iostream>
#include "Particle.h"
#include "CompContainer.h"

void Particle::preHandle()
{
	if (xvel == 0 && yvel == 0) {
		return;
	}

	if (yvel < 10) {
		yvel++;
	}

	flat2d::CollisionDetector& colDetector = CompContainer::getInstance().getCollisionDetector();

	GameObject *object;
	xpos += xvel;
	ypos += yvel;
	if ((object = colDetector.checkForCollisions(this)) != NULL) {
		xvel = 0;
		yvel = 0;
		object = NULL;
	}
}

void Particle::render(SDL_Renderer* renderer) const
{
	Camera& cam = CompContainer::getInstance().getCamera();

	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_Rect box = { cam.getScreenXposFor(xpos), cam.getScreenYposFor(ypos), 2, 2 };
	SDL_RenderFillRect( renderer, &box );
}

SDL_Rect Particle::getBoundingBox() const
{
	SDL_Rect box = { xpos, ypos, 1, 1 };
	return box;
}

bool Particle::isDead() const
{
	return timer.getTicks() > 10000;
}
