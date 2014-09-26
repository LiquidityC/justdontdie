#include "ResourceContainer.h"

ResourceContainer::~ResourceContainer()
{
	clearTextures();
}

void ResourceContainer::clearTextures()
{
	for (auto it = textures.begin(); it != textures.end(); it++) {
		SDL_DestroyTexture(*it);
	}
	textures.clear();
}

void ResourceContainer::addTexture(SDL_Texture* texture)
{
	textures.push_back(texture);
}
