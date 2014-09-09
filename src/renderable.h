#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include "object.h"

class SDL_Surface;

class Renderable : public Object
{
	public:
		virtual ~Renderable() {};
		virtual void render(SDL_Surface&) const = 0;
		virtual void preRender() = 0;
		virtual void postRender() = 0;
};

#endif
