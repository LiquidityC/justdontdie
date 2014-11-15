#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <flat/flat.h>

class Particle : public flat2d::RenderedGameObject
{
	protected:
		float xvel, yvel;
		flat2d::Timer deathTimer;

		SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;

	public:
		Particle(int x, int y, int w, int h, int xv, int yv) :
			RenderedGameObject(x, y, w, h),
			xvel(xv),
			yvel(yv) { 
				setDead(false);
				deathTimer.start();
			};

		virtual void preRender(const flat2d::GameData*);
		virtual void render(const flat2d::RenderData*) const;
		virtual void postRender(const flat2d::GameData*);

	protected:
		virtual void setRenderDrawColor(SDL_Renderer*) const = 0;
		void reduceXVel(int reduction, float deltaTime);
		void reduceYVel(int reduction, float deltaTime);
		float getReducedVelocity(float vel, int reduction, float deltaTime);

		void setBlendMode(SDL_BlendMode);
};

#endif
