#ifndef _BOT_H
#define _BOT_H

#include <flat/flat.h>

class Bot : public flat2d::RenderedGameObject
{
	typedef struct {
		int x, y;
	} Clip;

	private:
		bool grounded = false;

		float xvel = 0;
		float yvel = 0;

		SDL_Rect clips[2] = {
			{ 	0,	 0, 37, 48 },
			{ 	37,	 0, 37, 48}
		};

		SDL_Texture* botTexture = NULL;

	public:

		Bot(unsigned int x, unsigned int y) : RenderedGameObject(x, y, 37, 48) { };

		~Bot() {
			if (texture != NULL) {
				SDL_DestroyTexture(botTexture);
				botTexture = NULL;
			}
		};

		void init(const flat2d::RenderData*);

		void handle(const SDL_Event& event);
		void postHandle();

		void preRender(const flat2d::RenderData*);
};

#endif
