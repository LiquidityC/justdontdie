#ifndef _BUTTON_H
#define _BUTTON_H

#include "RenderedGameObject.h"

namespace flat2d
{
	namespace ui
	{
		class Button : public flat2d::RenderedGameObject
		{
			private:
				std::function<void ()> onClick;
				bool mouseIsOver = false;

			public:
				Button(int x, int y, unsigned int w, unsigned int h, std::function<void ()> click) 
					: RenderedGameObject(x, y, w, h), onClick(click) { 
						setCollidable(false);
					};

				~Button() { };

				void setOnClick(std::function<void ()> click);
				void triggerClick();

				virtual void render(const RenderData* data) const;

				virtual void handle(const SDL_Event&);
		};

	}
}

#endif
