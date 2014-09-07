#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include <SDL2/SDL.h>

class EventHandler
{
	public:
		virtual ~EventHandler() {};
		virtual void handle(SDL_Event&) = 0;
};

#endif
