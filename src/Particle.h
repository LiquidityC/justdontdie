#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <flat/flat.h>

class Particle : public flat2d::GameObject
{
	protected:
		int xpos, ypos, width, height;
		float xvel, yvel;
		bool dead;

	public:
		Particle(int x, int y, int w, int h, int xv, int yv) :
			GameObject(),
			xpos(x),
			ypos(y),
			width(w),
			height(h),
			xvel(xv),
			yvel(yv),
			dead(false) { };

		virtual void preHandle();
		virtual void handle(const SDL_Event& event) { };
		virtual void postHandle() { };

		virtual void preRender() { };
		virtual void render(SDL_Renderer*) const;
		virtual void postRender() { }; // Do nothing, override

		virtual bool isCollider() const { return false; }
		virtual SDL_Rect getBoundingBox() const;

		bool isDead() const;

	protected:
		void setDead(bool);
		virtual void setRenderDrawColor(SDL_Renderer*) const = 0;
};

#endif
