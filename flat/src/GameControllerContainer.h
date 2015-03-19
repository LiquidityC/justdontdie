#ifndef _GAME_CONTROLLER_CONTAINER_H
#define _GAME_CONTROLLER_CONTAINER_H

#include <vector>
#include "GameController.h"

namespace flat2d
{
	class GameControllerContainer
	{
		private:
			std::vector<GameController*> controllers;

			int loadControllers();

		public:
			GameControllerContainer();
			virtual ~GameControllerContainer();

			bool hasControllers();
			GameController* getController(int);
	};
}

#endif
