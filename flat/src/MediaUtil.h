#ifndef MEDIAUTIL_H_
#define MEDIAUTIL_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

namespace flat2d
{
	class MediaUtil
	{
		public:
			static SDL_Texture* loadTexture(std::string, SDL_Renderer*);
			static SDL_Texture* loadTextTexture(std::string, std::string, SDL_Renderer*);
	};
}
#endif // MEDIAUTIL_H_
