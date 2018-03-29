#ifndef FRAMECOUNTER_H_
#define FRAMECOUNTER_H_

#include <flat/flat.h>
#include <string>
#include <vector>

class FrameCounter : public flat2d::Entity
{
	using Textures = std::vector<flat2d::Texture*>;

	private:
		int xpos = 10,
			ypos = 10;

		unsigned int addedTextures = 0;

		flat2d::Timer fpsTimer;
		flat2d::Timer updateTimer;
		SDL_Color color = { 0, 0, 0 };

		flat2d::Texture *avgFpsText = nullptr;
		flat2d::Texture *avgFpsCount = nullptr;

		flat2d::Texture *fpsText = nullptr;
		flat2d::Texture *fpsCount = nullptr;

		flat2d::Texture *dtText = nullptr;
		flat2d::Texture *dtCount = nullptr;

		flat2d::Texture *objText = nullptr;
		flat2d::Texture *objCount = nullptr;

		flat2d::Texture *colText = nullptr;
		flat2d::Texture *colCount = nullptr;

		Textures textures;

		int countedFrames = 0;
		int currentFps = 0;

		flat2d::Texture* createTexture(std::string text, SDL_Renderer*);

		void renderTexture(const flat2d::Texture *, int x, int y, const flat2d::RenderData*) const;

	public:
		FrameCounter(int x, int y) : Entity( x, y, 300, 300 ) {
			setFixedPosition(true);
			entityProperties.setCollidable(false);
		}

		~FrameCounter() {
			if (fpsText)
				delete fpsText;
			if (fpsCount)
				delete fpsCount;
			if (avgFpsText)
				delete avgFpsText;
			if (avgFpsCount)
				delete avgFpsCount;
			if (dtText)
				delete dtText;
			if (dtCount)
				delete dtCount;
			if (objText)
				delete objText;
			if (objCount)
				delete objCount;
			for (auto it : textures) {
				delete it;
			}
		}

		void init(const flat2d::GameData*);
		void preRender(const flat2d::GameData*);
		void checkDynamicExecutionTimes(const flat2d::GameData*);
		void render(const flat2d::RenderData*) const;
};

#endif // FRAMECOUNTER_H_
