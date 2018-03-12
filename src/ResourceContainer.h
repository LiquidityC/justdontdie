#ifndef RESOURCECONTAINER_H_
#define RESOURCECONTAINER_H_

#include <SDL.h>
#include <vector>
#include <memory>

/**
 * Class that holds resources and properly disposes of them at exit.
 */
class ResourceContainer
{
	private:
		std::vector<SDL_Texture*> textures;

	public:
		ResourceContainer() { }
		~ResourceContainer();

		void clearTextures();

		void addTexture(SDL_Texture* texture);
};

#endif // RESOURCECONTAINER_H_
