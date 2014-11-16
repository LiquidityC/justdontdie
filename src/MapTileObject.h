#ifndef _GENERIC_GAME_OBJECT_H
#define _GENERIC_GAME_OBJECT_H

#include "NewMacro.h"
#include <iostream>
#include <flat/flat.h>
#include "GameObjectType.h"

class MapTileObject : public flat2d::RenderedGameObject
{
	private:
		flat2d::Timer launchTimer;
		std::map<std::string, bool> properties;

	public:
		MapTileObject(int x, int y, int w, int h, SDL_Texture* t) 
			: RenderedGameObject(x, y, w, h), launchTimer() { 
				setTexture(t);
			};

		int getType() {
			return GameObjectType::TILE;
		}

		void preRender(const flat2d::GameData*);

		void setProperty(std::string, bool);
		bool hasProperty(std::string) const;
};

#endif
