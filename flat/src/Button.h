#ifndef _BUTTON_H
#define _BUTTON_H

#include "../src/GameObject.h"

namespace flat2d
{
	namespace ui
	{
		class Button : public flat2d::GameObject
		{
			private:
				int xpos, ypos;
				int width, height;
				std::function<void ()> onClick;
				bool mouseIsOver = false;

			public:
				Button(int x, int y, unsigned int w, unsigned int h, std::function<void ()> click) 
					: GameObject(), xpos(x), ypos(y), width(w), height(h), onClick(click) { };

				~Button() { };

				void setOnClick(std::function<void ()> click);
				void triggerClick();

				virtual void preRender() { };
				virtual void render(SDL_Renderer* renderer) const;
				virtual void postRender() { };

				virtual void preHandle() { };
				virtual void handle(const SDL_Event&);
				virtual void postHandle() { };

				void init(SDL_Renderer* renderer) { };
				bool isDead() const { return false; };
				bool isCollider() const { return false; };
				SDL_Rect getBoundingBox() const { SDL_Rect box = { xpos, ypos, width, height }; return box; };
		};

	}
}

#endif
