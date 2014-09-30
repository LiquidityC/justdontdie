#ifndef _GAME_OBJECT_IMPL_H
#define _GAME_OBJECT_IMPL_H

#include "../src/GameObject.h"

class GameObjectImpl : public flat2d::GameObject
{
	private:
		static const unsigned int WIDTH = 10;
		static const unsigned int HEIGHT = 10;

		int xpos, ypos;

		bool dead = false;

	public:
		GameObjectImpl(unsigned int x, unsigned int y) : GameObject(), xpos(x), ypos(y) { };

		void init(SDL_Renderer* renderer, flat2d::Camera *camera) { };

		void preHandle() { }; // Do nothing, override
		void handle(const SDL_Event& event) { } // Do nothing, override
		void postHandle() { }; // Do nothing, override

		void preRender() { }; // Do nothing, override
		void render(SDL_Renderer* renderer) const { }; // Nothing, we don't render in test
		void postRender() { }; // Do nothing, override

		bool isCollider() const;
		SDL_Rect getBoundingBox() const;

		void init(SDL_Renderer* renderer) { };
		void setDead(bool b);
		bool isDead() const;

		const SDL_Texture* getTexture() const { return NULL; };
		void setTexture(SDL_Texture* texture) { };
};

#endif
