#ifndef _BLOCK_H
#define _BLOCK_H

#include <flat/GameObject.h>

class Block : public flat2d::GameObject
{
	private:
		static const unsigned int WIDTH = 10;
		static const unsigned int HEIGHT = 10;

		int xpos, ypos;

		SDL_Texture* texture = NULL;

	public:
		Block(unsigned int x, unsigned int y) : GameObject(), xpos(x), ypos(y) { };

		~Block() {
			if (texture != NULL) {
				SDL_DestroyTexture(texture);
				texture = NULL;
			}
		}

		void init(SDL_Renderer* renderer);

		void preHandle() { }; // Do nothing, override
		void handle(const SDL_Event& event) { } // Do nothing, override
		void postHandle() { }; // Do nothing, override

		void preRender() { }; // Do nothing, override
		void render(SDL_Renderer* renderer) const;
		void postRender() { }; // Do nothing, override

		bool isCollider() const;
		SDL_Rect getBoundingBox() const;
};

#endif
