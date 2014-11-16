#ifndef _GAME_SETTINGS_H
#define _GAME_SETTINGS_H

#include <Box2D/Box2D.h>

class GameSettings
{
	public:
		static const int SCREEN_WIDTH 			= 800;
		static const int SCREEN_HEIGHT 			= 600;

		static const int SCREEN_FPS 			= 60;
		static const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

		// Box2d settings
		static constexpr float32 B2_TIME_STEP	= 1 / SCREEN_FPS;
		static constexpr int32 B2_VEL_ITER		= 6;
		static constexpr int32 B2_POS_ITER		= 2;
};

#endif
