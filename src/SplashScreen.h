#ifndef SPLASHSCREEN_H_
#define	SPLASHSCREEN_H_

#include <flat/flat.h>

/**
 * NOTE(Linus): This isn't used yet. But I don't want to srop it because it definitely will be used.
 */

class SplashScreen : public flat2d::Entity
{
	private:
		flat2d::Timer showTimer;
};

#endif // SPLASHSCREEN_H_
