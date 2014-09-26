#ifndef _RESOURCE_CONTAINER_H
#define _RESOURCE_CONTAINER_H

#include <SDL2/SDL.h>
#include <vector>

/**
 * Class that holds resources and properly disposes of them at exit.
 */
class ResourceContainer
{
	private:
		std::vector<SDL_Texture*> textures;

	public:
		ResourceContainer() { };
		~ResourceContainer();

		void clearTextures();

		void addTexture(SDL_Texture*);
};

#endif
