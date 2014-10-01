#ifndef _GAME_OBJECT_IMPL_H
#define _GAME_OBJECT_IMPL_H

#include "../src/RenderedGameObject.h"

class GameObjectImpl : public flat2d::RenderedGameObject
{
	public:
		GameObjectImpl(unsigned int x, unsigned int y) : RenderedGameObject(x, y, 10, 10) { 
			setCollidable(true);
		};
};

#endif
