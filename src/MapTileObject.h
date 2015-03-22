#ifndef MAPTILEOBJECT_H_
#define MAPTILEOBJECT_H_

#include <flat/flat.h>
#include <iostream>
#include <map>
#include <string>
#include "EntityType.h"

class MapTileObject : public flat2d::Entity
{
	private:
		flat2d::Timer launchTimer;
		std::map<std::string, bool> properties;

	public:
		MapTileObject(int x, int y, int w, int h, SDL_Texture* t)
			: Entity(x, y, w, h), launchTimer() {
				setTexture(t);
			}

		int getType() {
			return EntityType::TILE;
		}

		void preRender(const flat2d::GameData*);

		void setProperty(std::string, bool);
		bool hasProperty(std::string) const;
};

#endif // MAPTILEOBJECT_H_
