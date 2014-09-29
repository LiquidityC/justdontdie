#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <flat/flat.h>

class Particle : public flat2d::GameObject
{
	private:
		int xpos, ypos, xvel, yvel;
		flat2d::Timer timer;

	public:
		Particle(int x, int y, int xv, int yv) 
			: GameObject(), xpos(x), ypos(y), xvel(xv), yvel(yv)  { 
				timer.start();
			}

		void preHandle(); // Do nothing, override
		void handle(const SDL_Event& event) { };
		void postHandle() { };

		void preRender() { };
		void render(SDL_Renderer*) const;
		void postRender() { }; // Do nothing, override

		bool isCollider() const { return false; }
		SDL_Rect getBoundingBox() const;

		bool isDead() const;
};

#endif
