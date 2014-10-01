#include <iostream>
#include <SDL2/SDL_ttf.h>
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

SDL_Texture* flat2d::MediaUtil::loadTextTexture(std::string text, std::string ttfPath, SDL_Renderer* renderer)
{
	TTF_Font *font = TTF_OpenFont(ttfPath.c_str(), 12);
	if (font == NULL) {
		std::cerr << "Failed to load font: " << ttfPath << " : " << TTF_GetError() << std::endl;
		return NULL;
	}

	SDL_Color color = { 255, 0, 0 };
	SDL_Surface* imgSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (imgSurface == NULL) {
		std::cerr << "Failed to load text image: " << ttfPath << std::endl;
		return NULL;
	}

	SDL_Texture* texture = NULL;
	texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
	if (texture == NULL) {
		std::cerr << "Unable to create text texture for: " << ttfPath << std::endl;
	}
	SDL_FreeSurface(imgSurface);

	return texture;
}
