#include <flat/flat.h>
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

		ss << gameData->getEntityContainer()->getObjectCount();
		objCount->loadFromRenderedText(ss.str(), color, renderer);
		ss.str("");

		ss << gameData->getEntityContainer()->getCollidablesCount();
		colCount->loadFromRenderedText(ss.str(), color, renderer);
		ss.str("");

		ss.flush();

		checkDynamicExecutionTimes(gameData);

		currentFps = 0;
		updateTimer.stop();
		updateTimer.start();
	}


	countedFrames++;
	currentFps++;
}

void FrameCounter::checkDynamicExecutionTimes(const flat2d::GameData* gameData)
{
	SDL_Renderer *renderer = gameData->getRenderData()->getRenderer();
	const flat2d::RuntimeAnalyzer::FloatMap* data = flat2d::RuntimeAnalyzer::getAvgTimes();
	if (data->size() > addedTextures) {
		std::stringstream ss;
		for (auto &functionTime : *data) {
			ss << functionTime.first << ": " << functionTime.second;
			flat2d::Texture *texture = createTexture(ss.str(), renderer);
			textures.push_back(texture);
			addedTextures++;
			ss.str("");
		}
		ss.flush();
	}

	int counter = 0;
	std::stringstream ss;
	for (auto &functionTime : *data) {
		ss << functionTime.first << ": " << std::fixed << std::setprecision(5) <<  functionTime.second;
		textures[counter]->loadFromRenderedText(ss.str(), color, renderer);
		ss.str("");
		counter++;
	}
	ss.flush();
}

void FrameCounter::init(const flat2d::GameData *gameData)
{
	SDL_Renderer *renderer = gameData->getRenderData()->getRenderer();

	avgFpsText = createTexture("AVG FPS:", renderer);
	avgFpsCount = createTexture("0", renderer);

	fpsText = createTexture("FPS:", renderer);
	fpsCount = createTexture("0", renderer);

	dtText = createTexture("DELTATIME:", renderer);
	dtCount = createTexture("0", renderer);

	objText = createTexture("OBJECTS:", renderer);
	objCount = createTexture("0", renderer);

	colText = createTexture("COLLIDABLES:", renderer);
	colCount = createTexture("0", renderer);
}

flat2d::Texture* FrameCounter::createTexture(std::string text, SDL_Renderer *renderer)
{
	flat2d::Texture *texture = new flat2d::Texture();
	texture->loadFont("resources/font/font.ttf", 12);
	texture->loadFromRenderedText(text, color, renderer);
	return texture;
}

void FrameCounter::renderTexture(const flat2d::Texture *t,
								 int x,
								 int y,
								 const flat2d::RenderData *data
								 ) const
{
	int w = t->getWidth();
	int h = t->getHeight();
	SDL_Rect box = { x, y, w, h };

	t->render(data->getRenderer(), nullptr, &box);
}

void FrameCounter::render(const flat2d::RenderData *renderData) const
{
	int x = xpos, y = ypos;

	renderTexture(avgFpsText, x, y, renderData);
	renderTexture(avgFpsCount, x + 90, y, renderData);

	renderTexture(fpsText, x, y += 15, renderData);
	renderTexture(fpsCount, x + 90, y, renderData);

	renderTexture(dtText, x, y += 15, renderData);
	renderTexture(dtCount, x + 90, y, renderData);

	renderTexture(objText, x, y += 15, renderData);
	renderTexture(objCount, x + 90, y, renderData);

	renderTexture(colText, x, y += 15, renderData);
	renderTexture(colCount, x + 90, y, renderData);

	for (auto texture : textures) {
		y += 15;
		renderTexture(texture, x, y, renderData);
	}
}
