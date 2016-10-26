#ifndef SPLASHSCREEN_H_
#define	SPLASHSCREEN_H_

#include <flat/flat.h>
#include <string>

class SplashScreen : public flat2d::Entity
{
	private:
		flat2d::Timer showTimer;
		std::string filename;

	public:
		explicit SplashScreen(std::string fname) : Entity(204, 136, 592, 528), filename(fname) {
			entityProperties.setCollidable(false);
		}

		void init(const flat2d::GameData *gameData);
};

#endif // SPLASHSCREEN_H_
