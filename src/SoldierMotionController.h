#ifndef _SOLDIER_MOTION_CONTROLLER_H
#define _SOLDIER_MOTION_CONTROLLER_H

#include <SDL2/SDL.h>
#include <flat/flat.h>

class Soldier;

class SoldierMotionController
{
	private:

		void stop(Soldier&);
		void moveLeft(Soldier&);
		void moveRight(Soldier&);
		void jump(Soldier&);
		void ghostMode(Soldier&);

	public:
		void handle(const SDL_Event&, Soldier& soldier);
		void postHandle(const flat2d::GameData*, Soldier& soldier);
};

#endif
