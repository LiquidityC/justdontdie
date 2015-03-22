#ifndef _GHOST_OVERLAY_H
#define _GHOST_OVERLAY_H

#include <flat/flat.h>
#include "GameSettings.h"

class GhostOverlay : public flat2d::Entity
{
	private:
		bool visible = false;

	public:
		GhostOverlay() : Entity(0, 0, GameSettings::SCREEN_WIDTH, GameSettings::SCREEN_HEIGHT) { }

		void render(const flat2d::RenderData*) const;

		bool isVisible();

		void setVisible(bool);
};

#endif
