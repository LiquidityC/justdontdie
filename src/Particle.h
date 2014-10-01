#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <flat/flat.h>

class Particle : public flat2d::RenderedGameObject
{
	protected:
		float xvel, yvel;

	public:
		Particle(int x, int y, int w, int h, int xv, int yv) :
			RenderedGameObject(x, y, w, h),
			xvel(xv),
			yvel(yv) { 
				setDead(false);
			};

		virtual void preRender(const flat2d::RenderData*);
		virtual void render(const flat2d::RenderData*) const;

	protected:
		virtual void setRenderDrawColor(SDL_Renderer*) const = 0;
};

#endif
