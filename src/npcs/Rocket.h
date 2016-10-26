#ifndef NPCS_ROCKET_H_
#define NPCS_ROCKET_H_

#include <flat/flat.h>
#include "../EntityType.h"

class Rocket : public flat2d::Entity
{
	public:
		enum Mode {
			NORMAL,
			GHOST,
			MULTI
		};

	private:
		flat2d::Timer deathTimer;
		flat2d::Timer switchTimer;
		SDL_Rect clip;
		Mode mode;

	public:
		Rocket(int x, int y, Mode m = Mode::NORMAL, bool rightToLeft = true) :
			flat2d::Entity(x, y, 24, 15), mode(m) {
				entityProperties.setXvel(rightToLeft ? -300 : 300);
				entityProperties.setCollidable(true);
				entityProperties.setColliderShape({ 5, 5, 15, 5 });

				int xclip = rightToLeft ? 0 : 24;
				int yclip = mode == Mode::GHOST ? 15 : 0;

				clip = { xclip, yclip, 24, 15 };
			}

		~Rocket() { }

		int getType() const {
			return EntityType::ROCKET;
		}

		void init(const flat2d::GameData*);

		void preMove(const flat2d::GameData*);
		void render(const flat2d::RenderData*) const;
		void postRender(const flat2d::GameData*);

		bool onCollision(Entity *collider, const flat2d::GameData *data);

		Mode getMode() const;
};

#endif // NPCS_ROCKET_H_
