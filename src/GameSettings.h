#ifndef _GAME_SETTINGS_H
#define _GAME_SETTINGS_H

class GameSettings
{
	public:
		static const int SCREEN_WIDTH 			= 800;
		static const int SCREEN_HEIGHT 			= 600;

		static const int SCREEN_FPS 			= 120;
		static const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
};

#endif
