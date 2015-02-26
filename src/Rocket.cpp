#include "Rocket.h"
#include "GameObjectType.h"
#include "MapTileObject.h"

void Rocket::init(const flat2d::GameData *data)
{
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/rocket.png", 
				data->getRenderData()->getRenderer()));
	deathTimer.start();
}

void Rocket::preRender(const flat2d::GameData *data)
{
	locationProperty.setXpos(
			locationProperty.getXpos() + (xvel * data->getDeltatimeMonitor()->getDeltaTime()));

	flat2d::CollisionDetector *detector = data->getCollisionDetector();
	GameObject *o = detector->checkForCollisions(this);
	if (o && o->getType() == GameObjectType::TILE) {
		MapTileObject *tile = static_cast<MapTileObject*>(o);
		if (tile->hasProperty("rocketStopper")) {
			setDead(true);
		}
	}

	if (mode == Mode::MULTI) {
		if (!switchTimer.isStarted()) {
			switchTimer.start();
		}

		if (switchTimer.getTicks() > 100) {
			switchTimer.stop();
			switchTimer.start();
			clip.y = clip.y == 0 ? 15 : 0;
			setClip(clip);
		}
	}
}

void Rocket::postRender(const flat2d::GameData *data)
{
	if (!isDead()) {
		setDead(deathTimer.getTicks() > 5000);
	}
}

Rocket::Mode Rocket::getMode() const
{
	return mode;
}
