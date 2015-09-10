#include "SoldierPowerupContainer.h"

void SoldierPowerupContainer::setMode(Powerup powerup)
{
	ghostMode = false;
	bulletMode = false;

	switch (powerup) {
		case Powerup::GHOST:
			ghostMode = true;
			break;
		case Powerup::BULLET:
			bulletMode = true;
			break;
		default:
			break;
	}
}

bool SoldierPowerupContainer::isGhostMode() const
{
	return ghostMode;
}

bool SoldierPowerupContainer::isBulletMode() const
{
	return bulletMode;
}
