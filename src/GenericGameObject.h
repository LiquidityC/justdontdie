#ifndef _GENERIC_GAME_OBJECT_H
#define _GENERIC_GAME_OBJECT_H

#include <iostream>
#include <flat/flat.h>

class GenericGameObject : public flat2d::RenderedGameObject
{
	public:
		GenericGameObject(int x, int y, int w, int h, SDL_Texture* t) 
			: RenderedGameObject(x, y, w, h) { 
				setTexture(t);
			};
};

#endif
