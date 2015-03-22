#ifndef RESOURCELOADER_H_
#define RESOURCELOADER_H_

#include <flat/flat.h>

class ResourceLoader
{
	public:
		bool loadSoundEffects(flat2d::GameData*);

		bool loadMusic(flat2d::GameData*);
};

#endif // RESOURCELOADER_H_
