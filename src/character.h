#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "eventhandler.h"
#include "renderable.h"

class Character : public Renderable, public EventHandler
{
	private:
		static const unsigned int WIDTH = 10;
		static const unsigned int HEIGHT = 10;

		int xpos, ypos;

	public:

		Character(int x, int y) : xpos(x), ypos(y) { };

		void handle(SDL_Event& event) { }; // Do nothing @Override
		void preHandle() { }; // Do nothing @Override

		void postHandle();

		void render(SDL_Surface& surface) const;
		void preRender() { }; // Do nothing @Override
		void postRender() { }; // Do nothing @Override
};

#endif
