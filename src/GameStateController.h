#ifndef GAMESTATECONTROLLER_H_
#define	GAMESTATECONTROLLER_H_

#include <flat/flat.h>

class GameStateController : public flat2d::VirtualGameStateController
{
	private:
		bool reset = false;

	public:
		virtual ~GameStateController() { }
		void handle(const SDL_Event& event);
		void gameStateCheck(flat2d::GameData *gameData);

		void resetGame(flat2d::GameData *gameData);
};

#endif // GAMESTATECONTROLLER_H_
