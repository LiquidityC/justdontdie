#include <iostream>
#include <algorithm>
#include "SoldierMotionController.h"
#include "Soldier.h"
#include "SoundMappings.h"
#include "MoveUtil.h"

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
			use();
			break;
		case SDLK_a:
		case SDLK_LEFT:
			movementMap[MOVE_LEFT] = true;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			movementMap[MOVE_RIGHT] = true;
			break;
#ifdef DEBUG
		case SDLK_1: // Normal mode
			std::cout << "NORMAL MODE" << std::endl;
			soldier->getPowerupContainer()->setMode(Powerup::NORMAL);
			break;
		case SDLK_2: // Ghost mode
			std::cout << "GHOST MODE" << std::endl;
			soldier->getPowerupContainer()->setMode(Powerup::GHOST);
			break;
		case SDLK_3: // Bullet mode
			std::cout << "BULLET MODE" << std::endl;
			soldier->getPowerupContainer()->setMode(Powerup::BULLET);
			break;
		case SDLK_4: // Bullet mode
			std::cout << "REVERSE GRAVITY" << std::endl;
			reverseGravity = !reverseGravity;
			break;
#endif //DEBUG
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
			use();
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
	if (boostTimer.isStarted() && boostTimer.getTicks() > 200) {
		boosting = false;
		soldier->particleEmitters[soldier->BOOST_EMITTER]->setRunning(false);
	}
	if (soldier->grounded) {
		boostEnabled = true;
	}

	stop();

	if (movementMap[MOVE_LEFT]) {
		moveLeft();
	} else if (movementMap[MOVE_RIGHT]) {
		moveRight();
	}

	if (movementMap[FLOAT] && soldier->getPowerupContainer()->isGhostMode()) {
		soldier->floating = true;
	} else {
		soldier->floating = false;
	}

	applyGravity();
}

void SoldierMotionController::applyGravity()
{
	if (boosting) {
		return;
	}

	if (!reverseGravity) {
		flat2d::EntityProperties &entityProperties = soldier->getEntityProperties();
		MoveUtil::applyGravity(&entityProperties);
		if (soldier->floating) {
			MoveUtil::applyFloat(&entityProperties);
		}
	} else {
		flat2d::EntityProperties &entityProperties = soldier->getEntityProperties();
		float yvel = entityProperties.getYvel();
		if (yvel > -800) {
			entityProperties.setYvel(yvel - std::min(60, 800 + static_cast<int>(yvel)));
		}
		if (soldier->floating && yvel < -100) {
			entityProperties.setYvel(-100);
		}
	}
}

void SoldierMotionController::stop()
{
	if (boosting) {
		return;
	}
	soldier->getEntityProperties().setXvel(0);
}

void SoldierMotionController::moveLeft()
{
	if (boosting) {
		return;
	}

	soldier->getEntityProperties().setXvel(-300);
	soldier->facingLeft = true;
}

void SoldierMotionController::moveRight()
{
	if (boosting) {
		return;
	}

	soldier->getEntityProperties().setXvel(300);
	soldier->facingLeft = false;
}

void SoldierMotionController::use()
{
	flat2d::EntityProperties &props = soldier->getEntityProperties();
	SoldierPowerupContainer *powerupContainer = soldier->getPowerupContainer();
	bool boostAvailable = powerupContainer->isBulletMode();

	if (!boosting && boostEnabled && boostAvailable) {
		if (soldier->facingLeft) {
			props.setXvel(-2000);
		} else {
			props.setXvel(2000);
		}
		props.setYvel(0);
		boostTimer.start();
		boosting = true;
		boostEnabled = false;
		soldier->particleEmitters[soldier->BOOST_EMITTER]->setRunning(true);
	}
}
void SoldierMotionController::jump()
{
	SoldierPowerupContainer *powerupContainer = soldier->getPowerupContainer();

	bool doubleJumpAvailable = !powerupContainer->isGhostMode() && !soldier->doubleJumped;

	if ( soldier->grounded || doubleJumpAvailable ) {
		soldier->getEntityProperties().setYvel(reverseGravity ? 1050 : -1050);
		soldier->doubleJumped = soldier->grounded ? false : true;
		soldier->grounded = false;
		soldier->mixer->playEffect(Effects::JUMP);
	}
}

void SoldierMotionController::freeze()
{
	for (auto it = movementMap.begin(); it != movementMap.end(); it++) {
		it->second = false;
	}
}
