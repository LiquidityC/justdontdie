#ifndef _BOT_H
#define _BOT_H

#include <flat/GameObject.h>

class Bot : public flat2d::GameObject
{
	typedef struct {
		int x, y;
	} Clip;

	private:
		static const unsigned int WIDTH 	= 37;
		static const unsigned int HEIGHT 	= 48;

		int xpos, ypos;
		int xvel = 0;
		int yvel = 0;

		int clipIndex;
		Clip clips[2] = {
			{ 	0,	 0 },
			{ 	37,	 0 }
		};

		bool grounded = false;
		SDL_Texture* botTexture = NULL;

	public:

		Bot(unsigned int x, unsigned int y) : GameObject(), xpos(x), ypos(y), clipIndex(0) { };

		~Bot() {
			if (botTexture != NULL) {
				SDL_DestroyTexture(botTexture);
				botTexture = NULL;
			}
		};

		void init(SDL_Renderer*);

		void preHandle() { }; // Do nothing, override
		void handle(const SDL_Event& event);
		void postHandle();

		void preRender();
		void render(SDL_Renderer*) const;
		void postRender() { }; // Do nothing, override

		bool isCollider() const { return true; }
		SDL_Rect getBoundingBox() const;
};

#endif
