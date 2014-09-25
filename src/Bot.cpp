#include <flat/MediaUtil.h>
#include <iostream>
#include "Bot.h"
#include "CompContainer.h"

void Bot::init(SDL_Renderer* renderer)
{
	botTexture = flat2d::MediaUtil::loadTexture("resources/bot.png", renderer);
}

void Bot::handle(const SDL_Event& e)
{
	if (e.type != SDL_KEYDOWN) {
		return;
	}

	if (e.key.keysym.sym == SDLK_SPACE && yvel == 0) {
		yvel = -20;
		grounded = false;
	}
}

void Bot::postHandle()
{
	xvel = 0;

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_A]) {
		xvel = -5;
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		xvel = 5;
	}

}

void Bot::preRender()
{
	// Gravity
	if (yvel < 10 && !grounded) {
		yvel += 1;
	}

	xpos += xvel;
	ypos += yvel;

	if (xvel > 0) {
		clipIndex = 0;
	} else if (xvel < 0) {
		clipIndex = 1;
	}

	flat2d::CollisionDetector& colDetector = CompContainer::getInstance().getCollisionDetector();
	if (colDetector.checkForCollisions(this)) {
		using namespace std;
		cout << "Collided!" << endl;
		xpos -= xvel;
		ypos -= yvel;
		yvel = 0;
		xvel = 0;
	}

	CompContainer::getInstance().getCamera().centerOn(xpos + (WIDTH/2), ypos + (HEIGHT/2));
}

void Bot::render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

	Camera cam = CompContainer::getInstance().getCamera();
	SDL_Rect box = { cam.getScreenXposFor(xpos), cam.getScreenYposFor(ypos), WIDTH, HEIGHT };
	SDL_Rect clip = { clips[clipIndex].x, clips[clipIndex].y, WIDTH, HEIGHT };
	SDL_RenderCopy(renderer, botTexture, &clip, &box);
}

SDL_Rect Bot::getBoundingBox() const
{
	SDL_Rect rect = { xpos, ypos, WIDTH, HEIGHT };
	return rect;
}
