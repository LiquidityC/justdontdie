#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "eventhandler.h"
#include "renderable.h"

class Character : public Renderable, public EventHandler
{
	public:

		void handle(SDL_Event& event);

		void render(SDL_Surface& surface);
};

#endif
