#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <flat-2d/GameObject.h>

class Block : public GameObject
{
	private:
		static const unsigned int WIDTH = 10;
		static const unsigned int HEIGHT = 10;

		int xpos, ypos;

	public:
		Block(unsigned int x, unsigned int y) : xpos(x), ypos(y) { };

		void preHandle(); // Do nothing, override
		void handle(const SDL_Event& event) { } // Do nothing, override
		void postHandle() { }; // Do nothing, override

		void preRender() { }; // Do nothing, override
		void render(SDL_Surface& surface) const;
		void postRender() { }; // Do nothing, override

		bool isCollider() const;
		SDL_Rect getBoundingBox() const;
};

#endif
