#ifndef FRAMECOUNTER_H_
#define FRAMECOUNTER_H_

#include <flat/flat.h>

class FrameCounter : public flat2d::Entity
{
	private:
		flat2d::Timer fpsTimer;
		flat2d::Timer updateTimer;
		TTF_Font *font = nullptr;
		SDL_Color color = { 255, 0, 0 };

		int countedFrames = 0;
		int currentFps = 0;

	public:
		FrameCounter() : Entity( 10, 10, 95, 15 ) {
			setFixedPosition(true);

			font = flat2d::MediaUtil::loadFont("resources/font/font.ttf");
		}

		~FrameCounter() {
			if (texture != nullptr) {
				SDL_DestroyTexture(texture);
				texture = nullptr;
			}
		}

		void preRender(const flat2d::GameData*);
};

#endif // FRAMECOUNTER_H_
