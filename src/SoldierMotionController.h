#ifndef SOLDIERMOTIONCONTROLLER_H_
#define SOLDIERMOTIONCONTROLLER_H_

#include <SDL2/SDL.h>
#include <flat/flat.h>
#include <map>

class Soldier;

class SoldierMotionController
{
	friend class Soldier;

	private:
		enum MovementMapping {
			MOVE_LEFT,
			MOVE_RIGHT,
			FLOAT,
			LAST
		};

		typedef std::map<MovementMapping, bool> MovementMap;

		Soldier *soldier = nullptr;
		MovementMap movementMap;

		flat2d::Timer boostTimer;
		bool boosting = false;
		bool boostEnabled = true;
		bool reverseGravity = false;

		void keyDown(const SDL_KeyboardEvent&);
		void keyUp(const SDL_KeyboardEvent&);
		void controllerAxisMotion(const SDL_ControllerAxisEvent&);
		void controllerButtonDown(const SDL_ControllerButtonEvent&);
		void controllerButtonUp(const SDL_ControllerButtonEvent&);

		void stop();
		void moveLeft();
		void moveRight();
		void use();
		void jump();
		void ghostMode();

	public:
		explicit SoldierMotionController(Soldier* soldier);

		void handle(const SDL_Event&);
		void preMove(const flat2d::GameData*);
		void freeze();
		void applyGravity();
};

#endif // SOLDIERMOTIONCONTROLLER_H_
