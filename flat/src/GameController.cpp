#include <iostream>
#include "GameController.h"

namespace flat2d
{
	GameController::GameController(int index)
	{
		controller = SDL_JoystickOpen(index);
		if (controller == nullptr) {
			std::cout << "Unable to open controller (" << index << ")" << std::endl;
		}
	}

	GameController::~GameController()
	{
		if (controller != nullptr) {
			SDL_JoystickClose( controller );
		}
	}

	SDL_Joystick* GameController::getSDLController()
	{
		return controller;
	}
}
