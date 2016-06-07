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
			flat2d::Texture *texture = createTexture(xpos, ypos += 15, ss.str(), renderer);
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

	avgFpsText = createTexture(xpos, ypos, "AVG FPS:", renderer);
	avgFpsCount = createTexture(xpos + 90, ypos, "0", renderer);

	fpsText = createTexture(xpos, ypos += 15, "FPS:", renderer);
	fpsCount = createTexture(xpos + 90, ypos, "0", renderer);

	dtText = createTexture(xpos, ypos += 15, "DELTATIME:", renderer);
	dtCount = createTexture(xpos + 90, ypos, "0", renderer);

	objText = createTexture(xpos, ypos += 15, "OBJECTS:", renderer);
	objCount = createTexture(xpos + 90, ypos, "0", renderer);

	colText = createTexture(xpos, ypos += 15, "COLLIDABLES:", renderer);
	colCount = createTexture(xpos + 90, ypos, "0", renderer);
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
	objText->render(renderData->getRenderer());
	objCount->render(renderData->getRenderer());
	colText->render(renderData->getRenderer());
	colCount->render(renderData->getRenderer());

	for (auto texture : textures) {
		texture->render(renderData->getRenderer());
	}
}
