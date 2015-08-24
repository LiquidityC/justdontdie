#include <sstream>
#include <iomanip>
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

	if (static_cast<int>((updateTimer.getTicks() / 1000.f)) > 1 || avgFps == 0) {
		if (texture != nullptr) {
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}

		std::stringstream ss;
		ss << "AVG FPS: " << avgFps << " FPS: " << currentFps
			<< " DT: " << gameData->getDeltatimeMonitor()->getDeltaTime();

		texture = flat2d::MediaUtil::createTextTexture(ss.str(), font, color, gameData->getRenderData()->getRenderer());
		ss.flush();

		currentFps = 0;
		updateTimer.stop();
		updateTimer.start();
	}


	countedFrames++;
	currentFps++;
}
