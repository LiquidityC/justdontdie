#include "Block.h"

void Block::preHandle()
{
	ypos -= 5;
}

void Block::render(SDL_Surface& surface) const
{
	SDL_Rect box = { xpos, ypos, WIDTH, HEIGHT };
	SDL_FillRect(&surface, &box, SDL_MapRGB(surface.format, 0xFF, 0xFF, 0xFF));
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
