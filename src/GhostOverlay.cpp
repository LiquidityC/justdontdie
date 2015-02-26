#include <SDL2/SDL.h>
#include "GhostOverlay.h"

void GhostOverlay::render(const flat2d::RenderData *renderData) const
{
	if (!visible) {
		return;
	}
	SDL_Rect box { 0, 0, locationProperty.getWidth(), locationProperty.getHeight() };

	SDL_Renderer *renderer = renderData->getRenderer();
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_MOD);
	SDL_SetRenderDrawColor(renderer, 0x00, 0xEA, 0xFF, 0x40);
	SDL_RenderFillRect(renderer, &box);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

bool GhostOverlay::isVisible()
{
	return visible;
}

void GhostOverlay::setVisible(bool visible)
{
	this->visible = visible;
}
