#include <iostream>
#include "SoldierMotionController.h"
#include "Soldier.h"
#include "SoundMappings.h"

void SoldierMotionController::handle(const SDL_Event& e, Soldier& soldier)
{
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
			case SDLK_SPACE:
			case SDLK_h:
			case SDLK_k:
				jump(soldier);
				break;
			case SDLK_j:
				ghostMode(soldier);
				break;
			default:
				break;
		}
	} else if (e.type == SDL_JOYAXISMOTION) {
		if (e.jaxis.axis == 0) {
			if (e.jaxis.value > 3200) {
				moveRight(soldier);
			} else if (e.jaxis.value < -3200) {
				moveLeft(soldier);
			} else {
				stop(soldier);
			}
		}
	} else if (e.type == SDL_JOYHATMOTION) {
		if (e.jhat.value & SDL_HAT_RIGHT) {
			moveRight(soldier);
		} else if (e.jhat.value & SDL_HAT_LEFT) {
			moveLeft(soldier);
		} else {
			stop(soldier);
		}
	} else if (e.type == SDL_JOYBUTTONDOWN) {
		std::cout << "Button: " << static_cast<int>(e.jbutton.button) << std::endl;
		switch (e.jbutton.button) {
			case 0:
				jump(soldier);
				break;
			case 1:
				ghostMode(soldier);
				break;
			default:
				break;
		}
	}
}

void SoldierMotionController::postHandle(const flat2d::GameData* data, Soldier& soldier)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
	if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
		moveLeft(soldier);
	}
	if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
		moveRight(soldier);
	}
	if (currentKeyStates[SDL_SCANCODE_K] && soldier.ghostMode && !soldier.grounded && soldier.yvel > 5) {
		soldier.yvel = 5;
	}
}

void SoldierMotionController::stop(Soldier& soldier)
{
	soldier.xvel = 0;
}

void SoldierMotionController::moveLeft(Soldier& soldier)
{
	soldier.xvel = -300;
	soldier.facingLeft = true;
}

void SoldierMotionController::moveRight(Soldier& soldier)
{
	soldier.xvel = 300;
	soldier.facingLeft = false;
}

void SoldierMotionController::jump(Soldier& soldier)
{
	if( soldier.grounded || (!soldier.ghostMode && !soldier.doubleJumped) ) {
		soldier.yvel = -1050;
		soldier.doubleJumped = soldier.grounded ? false : true;
		soldier.grounded = false;
		soldier.mixer->playEffect(Effects::JUMP);
	}
}

void SoldierMotionController::ghostMode(Soldier& soldier)
{
	soldier.ghostMode = !soldier.ghostMode;
	soldier.ghostOverlay->setVisible(soldier.ghostMode);
}
