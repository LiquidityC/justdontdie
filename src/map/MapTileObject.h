#ifndef MAP_MAPTILEOBJECT_H_
#define MAP_MAPTILEOBJECT_H_

#include <flat/flat.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "EntityGenerator.h"
#include "../EntityType.h"
#include "../ParticleEmitter.h"


class MapTileObject : public flat2d::Entity
{
	private:
		std::map<std::string, std::string> properties;

		flat2d::Timer hiddenTimer;
		bool hidden = false;

		ParticleEmitter *tileBreakEmitter = nullptr;
		std::vector<EntityGenerator*> generators;

	public:
		MapTileObject(int x, int y, int w, int h, SDL_Texture* t)
			: Entity(x, y, w, h) {
				setTexture(t);
			}

		~MapTileObject() {
			if (tileBreakEmitter) { delete tileBreakEmitter; }
			for (auto generator : generators) {
				delete generator;
			}
		}

		void initTile();

		int getType() const {
			return EntityType::TILE;
		}

		void preMove(const flat2d::GameData*);
		void render(const flat2d::RenderData*) const;

		void setProperty(std::string, std::string);
		std::string getProperty(std::string) const;
		bool hasProperty(std::string) const;

		bool isHidden() const;
		void hide();

		void destroy(const flat2d::GameData*);
};

#endif // MAP_MAPTILEOBJECT_H_
