#ifndef MAP_MAPTILEOBJECT_H_
#define MAP_MAPTILEOBJECT_H_

#include <flat/flat.h>
#include <iostream>
#include <map>
#include <string>
#include "../EntityType.h"
#include "../ParticleEmitter.h"
#include "../map/RocketGenerator.h"

class MapTileObject : public flat2d::Entity
{
	private:
		std::map<std::string, bool> properties;

		flat2d::Timer hiddenTimer;
		bool hidden = false;

		ParticleEmitter *tileBreakEmitter = nullptr;
		RocketGenerator *rocketGenerator = nullptr;

		void init();

	public:
		MapTileObject(int x, int y, int w, int h, SDL_Texture* t)
			: Entity(x, y, w, h) {
				setTexture(t);
				init();
			}

		~MapTileObject() {
			if (tileBreakEmitter) { delete tileBreakEmitter; }
			if (rocketGenerator) { delete rocketGenerator; }
		}

		int getType() const {
			return EntityType::TILE;
		}

		void preMove(const flat2d::GameData*);
		void render(const flat2d::RenderData*) const;

		void setProperty(std::string, bool);
		bool hasProperty(std::string) const;

		bool isHidden() const;
		void hide();

		void destroy(const flat2d::GameData*);
};

#endif // MAP_MAPTILEOBJECT_H_
