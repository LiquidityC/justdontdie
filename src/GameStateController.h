#ifndef GAMESTATECONTROLLER_H_
#define	GAMESTATECONTROLLER_H_

#include <flat/flat.h>
#include <string>
#include <vector>

class GameStateController : public flat2d::VirtualGameStateController
{
	typedef struct {
		std::string path;
		std::string file;
	} MapData;

	private:
		bool reset = false;

		size_t currentMapIndex = 0;
		std::vector<MapData> maps;

		void initMaps();

	public:
		GameStateController();
		virtual ~GameStateController() { }

		// Overrides
		void handle(const SDL_Event& event);
		void gameStateCheck(flat2d::GameData *gameData);

		void resetGame(flat2d::GameData *gameData);

		void loadNextMap();
};

#endif // GAMESTATECONTROLLER_H_
