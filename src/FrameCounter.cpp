#include <sstream>
#include <iomanip>
#include <string>
#include "FrameCounter.h"

void FrameCounter::preRender(const flat2d::GameData *gameData)
{
	if (!fpsTimer.isStarted() || !updateTimer.isStarted()) {
		fpsTimer.start();
		updateTimer.start();
		return;
	}

	float avgFps = countedFrames / (fpsTimer.getTicks() / 1000.f);
	if (avgFps > 20000) {
		avgFps = 0;
	}

	if (static_cast<int>((updateTimer.getTicks() / 1000.f)) > 1) {
		SDL_Renderer *renderer = gameData->getRenderData()->getRenderer();
		std::stringstream ss;
		ss << avgFps;
		avgFpsCount->loadFromRenderedText(ss.str(), color, renderer);
		ss.str("");

		ss << currentFps;
		fpsCount->loadFromRenderedText(ss.str(), color, renderer);
		ss.str("");

		ss << gameData->getDeltatimeMonitor()->getDeltaTime();
		dtCount->loadFromRenderedText(ss.str(), color, renderer);
		ss.str("");
		ss.flush();

		currentFps = 0;
		updateTimer.stop();
		updateTimer.start();
	}


	countedFrames++;
	currentFps++;
}

void FrameCounter::init(const flat2d::GameData *gameData)
{
	SDL_Renderer *renderer = gameData->getRenderData()->getRenderer();

	avgFpsText = createTexture(10, 10, "AVG FPS:", renderer);
	avgFpsCount = createTexture(100, 10, "0", renderer);

	fpsText = createTexture(10, 25, "FPS:", renderer);
	fpsCount = createTexture(100, 25, "0", renderer);

	dtText = createTexture(10, 40, "DELTATIME:", renderer);
	dtCount = createTexture(100, 40, "0", renderer);
}

flat2d::Texture* FrameCounter::createTexture(int xpos, int ypos, std::string text, SDL_Renderer *renderer)
{
	flat2d::Texture *texture = new flat2d::Texture(xpos, ypos);
	texture->loadFont("resources/font/font.ttf", 12);
	texture->loadFromRenderedText(text, color, renderer);
	return texture;
}

void FrameCounter::render(const flat2d::RenderData *renderData) const
{
	avgFpsText->render(renderData->getRenderer());
	avgFpsCount->render(renderData->getRenderer());
	fpsText->render(renderData->getRenderer());
	fpsCount->render(renderData->getRenderer());
	dtText->render(renderData->getRenderer());
	dtCount->render(renderData->getRenderer());
}
