#include "Block.h"
#include "CompContainer.h"

void Block::preHandle()
{
}

void Block::render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );

	Camera cam = CompContainer::getInstance().getCamera();
	SDL_Rect boundingBox = getBoundingBox();
	if (!cam.isVisibleOnCamera(boundingBox)) {
		return;
	}
	SDL_Rect box = { cam.getScreenXposFor(xpos), cam.getScreenYposFor(ypos), WIDTH, HEIGHT };
	SDL_RenderFillRect(renderer, &box);
}

bool Block::isCollider() const
{
	return true;
}

SDL_Rect Block::getBoundingBox() const
{
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
	return box;
}
