#ifndef _ROCKET_H
#define _ROCKET_H

#include <flat/flat.h>
#include "GameObjectType.h"

class Rocket : public flat2d::RenderedGameObject
{
	public:
		enum Mode {
			NORMAL,
			GHOST,
			MULTI
		};

	private:
		float xvel;
		flat2d::Timer deathTimer;
		flat2d::Timer switchTimer;
		SDL_Rect clip;
		Mode mode; 

	public:
		Rocket(int x, int y, Mode m = Mode::NORMAL, bool rightToLeft = true) :
			flat2d::RenderedGameObject(x, y, 24, 15), mode(m) {
				xvel = rightToLeft ? -300 : 300;
				setCollidable(true);

				int xclip = rightToLeft ? 0 : 24;
				int yclip = mode == Mode::GHOST ? 15 : 0;

				clip = { xclip, yclip, 24, 15 };
				setClip(clip);
			};

		~Rocket() {
			if (texture != nullptr) {
				SDL_DestroyTexture(texture);
			}
		};

		int getType() {
			return GameObjectType::ROCKET;
		};

		void init(const flat2d::GameData*);

		void preRender(const flat2d::GameData*);
		void postRender(const flat2d::GameData*);

		Mode getMode() const;
};

#endif
