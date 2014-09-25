#include <iostream>
#include "MediaUtil.h"

SDL_Texture* flat2d::MediaUtil::loadTexture(std::string path, SDL_Renderer* renderer)
{
	SDL_Surface* imgSurface = IMG_Load(path.c_str());
	if (imgSurface == NULL) {
		std::cerr << "Failed to load image: " << path << std::endl;
		return NULL;
	}

	SDL_Texture* texture = NULL;
	texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
	if (texture == NULL) {
		std::cerr << "Unable to create texture from: " << path << std::endl;
	}
	SDL_FreeSurface(imgSurface);

	return texture;
}
