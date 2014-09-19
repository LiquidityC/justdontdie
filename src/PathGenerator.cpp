#include <iostream>
#include <cstdlib>

#include "PathGenerator.h"
#include "objectcontainer.h"
#include "Block.h"

void PathGenerator::generatePath(int screenWidth, int screenHeight)
{
	if (leftBlock == NULL && rightBlock == NULL) {
		leftBlock = new Block(0, screenHeight);
		rightBlock = new Block(screenWidth - leftBlock->getBoundingBox().w, screenHeight);

		SDL_Rect leftBox = leftBlock->getBoundingBox();
		SDL_Rect rightBox = rightBlock->getBoundingBox();

		pathMaxWidth = rightBox.x - (leftBox.x + leftBox.w);
		pathWidth = pathMaxWidth;

		objectContainer->registerObject(leftBlock);
		objectContainer->registerObject(rightBlock);

		return;
	} 

	SDL_Rect leftBox = leftBlock->getBoundingBox();
	SDL_Rect rightBox = rightBlock->getBoundingBox();

	// Is it time to render a new object?
	if ( (leftBox.y + leftBox.h) > screenHeight) {
		return;
	}

	int narrowingRatio = 0;
	if (pathWidth > pathGoalWidth) {
		pathWidth -= 10;
		narrowingRatio = 5;
	}

	// Do we turn
	if (rand() % 50 == 0 && narrowingRatio == 0) {

		// Where do we turn?
		switch (rand() % 3) {
			case 0:
				turningRatio = -5;
				break;
			case 1:
				turningRatio = 0;
				break;
			case 2:
				turningRatio = 5;
				break;
			default:
				break;

		};

	}

	if (leftBox.x < 0) {
		turningRatio *= -1;
	} else if (rightBox.x > (int) pathMaxWidth) {
		turningRatio *= -1;
	}

	// Do we narrow down?
	if (rand() % 100 == 0 && narrowingRatio == 0) {
		pathGoalWidth = (rand() % (screenWidth - 50) ) + 75;
		pathGoalWidth = pathGoalWidth > pathMaxWidth ? pathMaxWidth : pathGoalWidth;
	}

	leftBlock = new Block(leftBox.x + narrowingRatio + turningRatio, leftBox.y + leftBox.h);
	rightBlock = new Block(rightBox.x - narrowingRatio + turningRatio, rightBox.y + rightBox.h);

	objectContainer->registerObject(leftBlock);
	objectContainer->registerObject(rightBlock);
}
