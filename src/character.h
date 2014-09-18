#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "gameobject.h"

class Character : public GameObject
{
	private:
		static const unsigned int WIDTH = 10;
		static const unsigned int HEIGHT = 10;

		int xpos, ypos;

	public:
		Character(unsigned int x, unsigned int y) : xpos(x), ypos(y) { };

		void preHandle() { }; // Do nothing, override
		void handle(const SDL_Event& event) { } // Do nothing, override
		void postHandle();

		void preRender() { }; // Do nothing, override
		void render(SDL_Surface& surface) const;
		void postRender() { }; // Do nothing, override

		bool isCollider();
		SDL_Rect getBoundingBox();
};

#endif
