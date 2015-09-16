#ifndef MAPTILEOBJECT_H_
#define MAPTILEOBJECT_H_

#include <flat/flat.h>
#include <iostream>
#include <map>
#include <string>
#include "EntityType.h"
#include "ParticleEmitter.h"

class MapTileObject : public flat2d::Entity
{
	private:
		flat2d::Timer launchTimer;
		std::map<std::string, bool> properties;

		flat2d::Timer hiddenTimer;
		bool hidden = false;

		ParticleEmitter *tileBreakEmitter;

	public:
		MapTileObject(int x, int y, int w, int h, SDL_Texture* t)
			: Entity(x, y, w, h), launchTimer() {
				setTexture(t);
				tileBreakEmitter = new ParticleEmitter(ParticleType::FIRE_PARTICLE);
			}

		~MapTileObject() {
			delete tileBreakEmitter;
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

#endif // MAPTILEOBJECT_H_
