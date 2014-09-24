#ifndef _SHIP_H
#define _SHIP_H

#include <flat/GameObject.h>

class Ship : public flat2d::GameObject
{
	private:
		static const unsigned int WIDTH 	= 10;
		static const unsigned int HEIGHT 	= 10;

		int xpos, ypos;

	public:

		Ship(unsigned int x, unsigned int y) : GameObject(), xpos(x), ypos(y) { };

		void preHandle() { }; // Do nothing, override
		void handle(const SDL_Event& event) { } // Do nothing, override
		void postHandle();

		void preRender() { }; // Do nothing, override
		void render(SDL_Renderer*) const;
		void postRender() { }; // Do nothing, override

		bool isCollider() const { return true; }
		SDL_Rect getBoundingBox() const;
};

#endif
