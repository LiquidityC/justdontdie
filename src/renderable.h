#ifndef _RENDERABLE_H
#define _RENDERABLE_H

class SDL_Surface;

class Renderable
{
	public:
		virtual ~Renderable() {};
		virtual void render(SDL_Surface&) const = 0;
};

#endif
