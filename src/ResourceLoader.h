#ifndef _RESOURCE_LOADER_H
#define _RESOURCE_LOADER_H

#include <flat/flat.h>

class ResourceLoader
{
	public:
		bool loadSoundEffects(flat2d::GameData*);

		bool loadMusic(flat2d::GameData*);
};

#endif
