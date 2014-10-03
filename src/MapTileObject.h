#ifndef _GENERIC_GAME_OBJECT_H
#define _GENERIC_GAME_OBJECT_H

#include "NewMacro.h"
#include <iostream>
#include <flat/flat.h>
#include "GameObjectType.h"

class MapTileObject : public flat2d::RenderedGameObject
{
	private:
		std::map<std::string, bool> properties;

	public:
		MapTileObject(int x, int y, int w, int h, SDL_Texture* t) 
			: RenderedGameObject(x, y, w, h) { 
				setTexture(t);
			};

		int getType() {
			return GameObjectType::TILE;
		}

		void setCollidable(bool);
		bool isCollider() const;

		void setProperty(std::string, bool);
		bool hasProperty(std::string) const;
};

#endif
