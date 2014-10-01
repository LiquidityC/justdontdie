#ifndef _MEDIA_UTIL_H
#define _MEDIA_UTIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

namespace flat2d
{
	class MediaUtil
	{
		private:
			SDL_Texture* texture;

		public: 
			static SDL_Texture* loadTexture(std::string, SDL_Renderer*);
			static SDL_Texture* loadTextTexture(std::string, std::string, SDL_Renderer*);
	};
}
#endif
