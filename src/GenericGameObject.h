#ifndef _GENERIC_GAME_OBJECT_H
#define _GENERIC_GAME_OBJECT_H

#include <iostream>
#include <flat/GameObject.h>

class GenericGameObject : public flat2d::GameObject
{
	private:
		int xpos, ypos;
		int width, height;

		SDL_Texture* texture = NULL;
		int clipX;
		int clipY;
		bool collider;

	public:
		GenericGameObject(int x, int y, int w, int h, SDL_Texture* t) 
			: GameObject(), xpos(x), ypos(y), width(w), height(h) { 
				texture = t;
				clipX = 0;
				clipY = 0;
				collider = false;
			};

		void setCollidable(bool);
		bool isCollider() const;

		void setClipCoordinates(int x, int y);

		SDL_Rect getBoundingBox() const;
		void render(SDL_Renderer*) const;

		void preHandle() { }; // Do nothing, override
		void handle(const SDL_Event& event) { }; // Do nothing
		void postHandle() { }; // Do nothing

		void preRender() { }; // Do nothing
		void postRender() { }; // Do nothing, override
};


#endif
