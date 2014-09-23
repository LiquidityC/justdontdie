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
				unsigned int width, height;
				std::function<void ()> onClick;

			public:
				Button(int x, int y, unsigned int w, unsigned int h, std::function<void ()> click) 
					: xpos(x), ypos(y), width(w), height(h), onClick(click) { };

				~Button() { };

				void setOnClick(std::function<void ()> click);
				void triggerClick();

				virtual void preRender() { };
				virtual void render(SDL_Renderer* renderer) const;
				virtual void postRender() { };

				virtual void preHandle() { };
				virtual void handle(const SDL_Event&);
				virtual void postHandle() { };

		};

	}
}

#endif
