#ifndef _GAME_CONTROLLER_H
#define _GAME_CONTROLLER_H

#include <SDL2/SDL.h>

namespace flat2d
{
	class GameControllerContainer;

	class GameController
	{
		private:
			SDL_Joystick *controller = nullptr;

		public:
			GameController(int);

			~GameController();

			SDL_Joystick* getSDLController();
	};
}

#endif
