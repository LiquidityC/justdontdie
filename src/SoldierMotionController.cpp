#include <iostream>
#include "SoldierMotionController.h"
#include "Soldier.h"
#include "SoundMappings.h"

SoldierMotionController::SoldierMotionController(Soldier *soldier)
{
	this->soldier = soldier;

	// Prepare the movement map
	for (int mapping = MOVE_LEFT; mapping != LAST; mapping++) {
		movementMap[static_cast<MovementMapping>(mapping)] = false;
	}
}

void SoldierMotionController::handle(const SDL_Event& e)
{
	switch (e.type) {
		case SDL_KEYDOWN:
			keyDown(e.key);
			break;
		case SDL_KEYUP:
			keyUp(e.key);
			break;
		case SDL_CONTROLLERAXISMOTION:
			controllerAxisMotion(e.caxis);
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			controllerButtonDown(e.cbutton);
			break;
		case SDL_CONTROLLERBUTTONUP:
			controllerButtonUp(e.cbutton);
			break;
		default:
			break;
	}
}

void SoldierMotionController::keyDown(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym) {
		case SDLK_k:
			jump();
			movementMap[FLOAT] = true;
			break;
		case SDLK_j:
			ghostMode();
			break;
		case SDLK_a:
		case SDLK_LEFT:
			movementMap[MOVE_LEFT] = true;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			movementMap[MOVE_RIGHT] = true;
			break;
		default:
			break;
	}
}

void SoldierMotionController::keyUp(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym) {
		case SDLK_k:
			movementMap[FLOAT] = false;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			movementMap[MOVE_LEFT] = false;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			movementMap[MOVE_RIGHT] = false;
			break;
		default:
			break;
	}
}

void SoldierMotionController::controllerAxisMotion(const SDL_ControllerAxisEvent& e)
{
	if (e.axis == SDL_CONTROLLER_AXIS_LEFTX) {
		if (e.value > 3200) {
			movementMap[MOVE_RIGHT] = true;
		} else if (e.value < -3200) {
			movementMap[MOVE_LEFT] = true;
		} else {
			movementMap[MOVE_LEFT] = false;
			movementMap[MOVE_RIGHT] = false;
		}
	}
}

void SoldierMotionController::controllerButtonDown(const SDL_ControllerButtonEvent& e)
{
	switch (e.button) {
		case SDL_CONTROLLER_BUTTON_A:
			jump();
			movementMap[FLOAT] = true;
			break;
		case SDL_CONTROLLER_BUTTON_B:
			ghostMode();
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			movementMap[MOVE_RIGHT] = true;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			movementMap[MOVE_LEFT] = true;
			break;
		default:
			break;
	}
}

void SoldierMotionController::controllerButtonUp(const SDL_ControllerButtonEvent& e)
{
	switch (e.button) {
		case SDL_CONTROLLER_BUTTON_A:
			movementMap[FLOAT] = false;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			movementMap[MOVE_RIGHT] = false;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			movementMap[MOVE_LEFT] = false;
			break;
		default:
			break;
	}
}

void SoldierMotionController::preMove(const flat2d::GameData* data)
{
	stop();

	if (movementMap[MOVE_LEFT]) {
		moveLeft();
	} else if (movementMap[MOVE_RIGHT]) {
		moveRight();
	}

	if (movementMap[FLOAT] && soldier->ghostMode && !soldier->grounded) {
		soldier->floating = true;
	} else {
		soldier->floating = false;
	}
}

void SoldierMotionController::stop()
{
	soldier->getEntityProperties().setXvel(0);
}

void SoldierMotionController::moveLeft()
{
	soldier->getEntityProperties().setXvel(-300);
	soldier->facingLeft = true;
}

void SoldierMotionController::moveRight()
{
	soldier->entityProperties.setXvel(300);
	soldier->facingLeft = false;
}

void SoldierMotionController::jump()
{
	if( soldier->grounded || (!soldier->ghostMode && !soldier->doubleJumped) ) {
		soldier->getEntityProperties().setYvel(-1050);
		soldier->doubleJumped = soldier->grounded ? false : true;
		soldier->grounded = false;
		soldier->mixer->playEffect(Effects::JUMP);
	}
}

void SoldierMotionController::ghostMode()
{
	soldier->ghostMode = !soldier->ghostMode;
	soldier->ghostOverlay->setVisible(soldier->ghostMode);
}

void SoldierMotionController::freeze()
{
	for (auto it = movementMap.begin(); it != movementMap.end(); it++) {
		it->second = false;
	}
}
