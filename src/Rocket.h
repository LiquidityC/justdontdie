#ifndef _ROCKET_H
#define _ROCKET_H

#include <flat/flat.h>
#include "GameObjectType.h"

class Rocket : public flat2d::RenderedGameObject
{
	private:
		bool ghostMode = false;
		float xvel;
		flat2d::Timer deathTimer;

	public:
		Rocket(int x, int y, bool ghost = false, bool rightToLeft = true) :
			flat2d::RenderedGameObject(x, y, 24, 15),
			ghostMode(ghost) {
				xvel = rightToLeft ? -300 : 300;
				setCollidable(true);

				int xclip = rightToLeft ? 0 : 24;
				int yclip = ghostMode ? 15 : 0;

				SDL_Rect clip = { xclip, yclip, 24, 15 };
				setClip(clip);
			};

		~Rocket() {
			if (texture != NULL) {
				SDL_DestroyTexture(texture);
			}
		};

		int getType() {
			return GameObjectType::ROCKET;
		};

		void init(const flat2d::RenderData*);

		void preRender(const flat2d::RenderData*);
		void postRender(const flat2d::RenderData*);

		bool isGhost() const;
};

#endif
