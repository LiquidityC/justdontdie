#include "Block.h"

void Block::preHandle()
{
	ypos -= 5;
}

void Block::render(SDL_Renderer* renderer) const
{
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
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
