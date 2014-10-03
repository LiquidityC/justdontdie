#ifndef _ROCKET_H
#define _ROCKET_H

#include <flat/flat.h>
#include "GameObjectType.h"

class Rocket : public flat2d::RenderedGameObject
{
	private:
		bool ghostMode = false;
		float xvel;

	public:
		Rocket(int x, int y, bool ghost = false, bool leftToRight = true) :
			flat2d::RenderedGameObject(x, y, 32, 32),
			ghostMode(ghost) {
				xvel = leftToRight ? -300 : 300;
				setCollidable(true);

				int xclip = leftToRight ? 0 : 32;
				int yclip = ghostMode ? 32 : 0;

				SDL_Rect clip = { xclip, yclip, 32, 32 };
				setClip(clip);
			};

		int getType() {
			return GameObjectType::ROCKET;
		};

		void preRender(const flat2d::RenderData*);

		bool isGhost() const;
};

#endif
