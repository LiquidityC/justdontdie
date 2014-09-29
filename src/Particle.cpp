#include <iostream>
#include <algorithm>
#include "Particle.h"
#include "CompContainer.h"

void Particle::preHandle()
{
	flat2d::Camera& cam = CompContainer::getInstance().getCamera();
	float deltaTime = cam.getDeltaTime();
	if (xvel == 0 && yvel == 0) {
		return;
	}

	if (yvel < 800) {
		yvel += std::min(3600 * deltaTime, 800 - yvel);
	}

	flat2d::CollisionDetector& colDetector = CompContainer::getInstance().getCollisionDetector();

	xpos += (xvel * deltaTime);
	ypos += (yvel * deltaTime);

	if (colDetector.checkForCollisions(this)) {
		xvel = 0;
		yvel = 0;
	}
}

void Particle::render(SDL_Renderer* renderer) const
{
	flat2d::Camera& cam = CompContainer::getInstance().getCamera();

	setRenderDrawColor(renderer);
	SDL_Rect box = { cam.getScreenXposFor(xpos), cam.getScreenYposFor(ypos), width, height };
	SDL_RenderFillRect( renderer, &box );
}

SDL_Rect Particle::getBoundingBox() const
{
	SDL_Rect box = { xpos, ypos, width, height };
	return box;
}

void Particle::setDead(bool dead)
{
 	this->dead = dead;
}

bool Particle::isDead() const
{
	return dead;
}
