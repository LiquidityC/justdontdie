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

		void init(SDL_Renderer* rendere, flat2d::Camera *camera);

		virtual void preRender();
		virtual void render(SDL_Renderer*) const;

	protected:
		virtual void setRenderDrawColor(SDL_Renderer*) const = 0;
};

#endif
