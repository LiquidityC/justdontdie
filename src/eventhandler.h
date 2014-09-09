#ifndef _EVENT_HANDLER_H
#define _EVENT_HANDLER_H

#include <SDL2/SDL.h>
#include "object.h"

class EventHandler : public Object
{
	public:
		virtual ~EventHandler() {};
		virtual void handle(SDL_Event&) = 0;
		virtual void preHandle() = 0;
		virtual void postHandle() = 0;
};

#endif
