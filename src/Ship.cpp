#include "Ship.h"
#include "CompContainer.h"

void Ship::postHandle()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_A]) {
		xpos -= 3;
	}
	if (currentKeyStates[SDL_SCANCODE_D]) {
		xpos += 3;
	}

	CompContainer::getInstance().getCamera().centerOn(xpos + (WIDTH/2), ypos + (HEIGHT/2));
}

void Ship::render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor( renderer, 0x00, 0xCC, 0x00, 0xFF );

	Camera cam = CompContainer::getInstance().getCamera();
	SDL_Rect box = { cam.getScreenXposFor(xpos), cam.getScreenYposFor(ypos), WIDTH, HEIGHT };
	SDL_RenderFillRect(renderer, &box);
}

SDL_Rect Ship::getBoundingBox() const
{
	SDL_Rect rect = { xpos, ypos, WIDTH, HEIGHT };
	return rect;
}
