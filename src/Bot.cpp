#include <iostream>
#include <algorithm>
#include "Bot.h"
#include "CompContainer.h"
#include "BloodParticle.h"
#include "Layers.h"

void Bot::init(SDL_Renderer* renderer)
{
	botTexture = flat2d::MediaUtil::loadTexture("resources/bot.png", renderer);
}

void Bot::handle(const SDL_Event& e)
{
	if (e.type != SDL_KEYDOWN) {
		return;
	}

	if (e.key.keysym.sym == SDLK_SPACE) {
		if( yvel > -300 && yvel < 300) {
			yvel = -1000;
			grounded = false;
		}
	} else if (e.key.keysym.sym == SDLK_h) {
		Particle *p;
		for (auto i = 0; i < 100; i++) {
			p = new BloodParticle( 
					xpos + WIDTH + 5, 
					ypos - 10, 100 + (rand() % 1000), 
					100 + (rand() % 1000) * (rand() % 2 == 1 ? -1 : 1) );
			CompContainer::getInstance().getObjectContainer().registerObject(p, Layers::FRONT);
		}
	}
}

void Bot::postHandle()
{
	xvel = 0;

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_A]) {
		xvel = -300;
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		xvel = 300;
	}

}

void Bot::preRender()
{
	flat2d::CollisionDetector& colDetector = CompContainer::getInstance().getCollisionDetector();
	flat2d::Camera& camera = CompContainer::getInstance().getCamera();
	float deltaTime = camera.getDeltaTime();

	// Gravity
	if (yvel < 800) {
		yvel += std::min(3600 * deltaTime, 800 - yvel);
	}

	xpos += (xvel * deltaTime);
	GameObject *object;
	if ((object = colDetector.checkForCollisions(this)) != NULL) {
		xpos -= (xvel * deltaTime);

		// Completly reach the obstruction
		while (!colDetector.hasCollided(this, object)) {
			xpos += xvel > 0 ? 1 : -1;
		}
		xpos += xvel > 0 ? -1 : 1;
		xvel = 0;
		object = NULL;
	}

	ypos += (yvel * deltaTime);
	if ((object = colDetector.checkForCollisions(this)) != NULL) {
		ypos -= (yvel * deltaTime);

		// Completly ground the bot
		while (!colDetector.hasCollided(this, object)) {
			ypos += yvel > 0 ? 1 : -1;
		}
		ypos += yvel > 0 ? -1 : 1;
		if (yvel > 0) {
			grounded = true;
		}
		yvel = 0;
		object = NULL;
	}

	if (xvel > 0) {
		clipIndex = 0;
	} else if (xvel < 0) {
		clipIndex = 1;
	}


	camera.centerOn(xpos + (WIDTH/2), ypos + (HEIGHT/2));
}

void Bot::render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

	flat2d::Camera cam = CompContainer::getInstance().getCamera();
	SDL_Rect box = { cam.getScreenXposFor(xpos), cam.getScreenYposFor(ypos), WIDTH, HEIGHT };
	SDL_Rect clip = { clips[clipIndex].x, clips[clipIndex].y, WIDTH, HEIGHT };
	SDL_RenderCopy(renderer, botTexture, &clip, &box);
}

SDL_Rect Bot::getBoundingBox() const
{
	SDL_Rect rect = { xpos, ypos, WIDTH, HEIGHT };
	return rect;
}
