#include <algorithm>
#include "MoveUtil.h"

void MoveUtil::applyGravity(flat2d::EntityProperties *entityProperties)
{
	float yvel = entityProperties->getYvel();
	if (yvel < 800) {
		entityProperties->setYvel(yvel + std::min(60, 800 - static_cast<int>(yvel)));
	}
}

void MoveUtil::applyFloat(flat2d::EntityProperties *entityProperties)
{
	if (entityProperties->getYvel() > 100) {
		entityProperties->setYvel(100);
	}
}
