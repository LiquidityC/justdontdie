#include <iostream>
#include <algorithm>
#include <cmath>
#include "Soldier.h"
#include "BloodParticle.h"
#include "LayerService.h"
#include "map/MapTileObject.h"
#include "npcs/Rocket.h"
#include "ParticleEngine.h"
#include "SoundMappings.h"
#include "CustomGameData.h"

void Soldier::init(const flat2d::GameData *gameData)
{
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/soldier.png",
				gameData->getRenderData()->getRenderer()));
	SDL_Rect clip = { 0, 0, entityProperties.getWidth(), entityProperties.getHeight() };
	setClip(clip);
	mixer = gameData->getMixer();
	entityProperties.setCollidable(true);
}

void Soldier::handle(const SDL_Event& e)
{
	if (killed) {
		return;
	}

	motionController->handle(e);
}

void Soldier::postHandle(const flat2d::GameData *gameData)
{
	if (spawnGraceTimer.isStarted() && spawnGraceTimer.getTicks() > 1000) {
		spawnGraceTimer.stop();
	}
}

bool Soldier::isFalling() const
{
	if (!motionController->reverseGravity) {
		return entityProperties.getYvel() > 60;
	} else {
		return entityProperties.getYvel() < -60;
	}
}

bool Soldier::killedFromFalling(const flat2d::GameData *data)
{
	bool falling = isFalling();

	// Turn the timer off if we are floating
	if (floating && fallTimer.isStarted()) {
		fallTimer.stop();
		return false;
	}

	// Falling timer
	if (falling && !fallTimer.isStarted() && !floating) {
		fallTimer.start();
	} else if (!falling && fallTimer.isStarted()) {
		int ticks = fallTimer.getTicks();
		fallTimer.stop();
		if (ticks > 750) {
			kill(data);
			return true;
		}
	}

	return false;
}

void Soldier::preMove(const flat2d::GameData *data)
{
	SDL_Rect box = entityProperties.getBoundingBox();

	// Center camera and check if we are outside screen, below, left of, right of
	if (!killed) {
		flat2d::Camera *camera = data->getRenderData()->getCamera();
		camera->centerOn(box.x + (box.w/2), box.y + (box.h/2));
		int screenXpos = camera->getScreenXposFor(box.x);
		int screenYpos = camera->getScreenYposFor(box.y);
		if (screenXpos + box.w < 0 || screenXpos > GameSettings::SCREEN_WIDTH) {
			kill(data);
		} else if (screenYpos > GameSettings::SCREEN_HEIGHT) {
			kill(data);
		}
	}

	bloodEmitter->emit(data, box);
	ghostEmitter->emit(data, box);
	if (killed && deathTimer.getTicks() < 3000) {
		return;
	} else if (killed) {
		restoreAtCheckpoint();
	}

	motionController->preMove(data);
	boostEmitter->emit(data, box);
	formChangeEmitter->emit(data, box);

	if (grounded) {
		grounded = !isFalling();
	}
	if (killedFromFalling(data)) {
		return;
	}

	calculateCurrentClip();
}

void Soldier::render(const flat2d::RenderData* data) const
{
	if (killed) {
		return;
	}

#ifdef COLLISION_DBG
	SDL_SetRenderDrawColor(data->getRenderer(), 0xFF, 0x00, 0x00, 0xFF );
	const flat2d::EntityProperties::Areas currentAreas = entityProperties.getCurrentAreas();
	for(auto it = currentAreas.begin(); it != currentAreas.end(); it++) {
		SDL_Rect bounds = (*it).asSDLRect();
		bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
		bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
		SDL_RenderDrawRect( data->getRenderer(), &bounds );
	}
#endif

	Entity::render(data);
}

void Soldier::calculateCurrentClip()
{
	static unsigned int frameSwitch = 0;
	static bool clipSwitch = false;

	int x = 0;
	int y = 0;

	if (motionController->reverseGravity) {
		y = entityProperties.getHeight() * 3;
	}

	if (powerupContainer->isGhostMode()) {
		y += entityProperties.getHeight();
	} else if (powerupContainer->isBulletMode()) {
		y += entityProperties.getHeight() * 2;
	}

	if (facingLeft) {
		x = 2 * entityProperties.getWidth();
	}

	if (entityProperties.getXvel() < 0) {
		x = 2 * entityProperties.getWidth();
	} else if (entityProperties.getXvel() > 0) {
		x = 0;
	}

	if (entityProperties.getXvel() != 0) {
		frameSwitch++;
		if (frameSwitch >= 4) {
			clipSwitch = !clipSwitch;
			frameSwitch = 0;
		}
		if (grounded) {
			x += clipSwitch ? entityProperties.getWidth() : 0;
		} else {
			x += entityProperties.getWidth();
		}
	}

	SDL_Rect clip = { x, y, entityProperties.getWidth(), entityProperties.getHeight() };
	setClip(clip);
}

bool Soldier::onCollision(Entity *collider, const flat2d::GameData *data)
{
	return handleGeneralCollision(collider, data);
}

bool Soldier::handleGeneralCollision(flat2d::Entity *o, const flat2d::GameData* data)
{
	switch (o->getType()) {
		case EntityType::TILE:
			return handleGeneralTileCollision(static_cast<MapTileObject*>(o), data);
		case EntityType::ROCKET:
			return handleRocketCollision(static_cast<Rocket*>(o), data);
		case EntityType::PARTICLE:
			return true;
		case EntityType::ENEMY:
			kill(data);
			return true;
		default:
			break;
	}
	return false;
}

bool Soldier::handleGeneralTileCollision(MapTileObject *o, const flat2d::GameData* data)
{
	if (o->isHidden()) {
		return true;
	}

	if (o->hasProperty("deadly")) {
		kill(data);
		return true;
	}

	if (o->hasProperty("checkpoint")) {
		checkPointX = o->getEntityProperties().getXpos();
		checkPointY = o->getEntityProperties().getYpos();
		return true;
	}

	if (o->hasProperty("ghostPowerup")) {
		o->hide();
		powerupContainer->setMode(Powerup::GHOST);
		return true;
	} else if (o->hasProperty("bulletPowerup")) {
		o->hide();
		powerupContainer->setMode(Powerup::BULLET);
		return true;
	} else if (o->hasProperty("gravityUp")) {
		o->hide();
		motionController->reverseGravity = true;
		return true;
	} else if (o->hasProperty("gravityDown")) {
		o->hide();
		motionController->reverseGravity = false;
		return true;
	} else if (o->hasProperty("goal")) {
		static_cast<CustomGameData*>(data->getCustomGameData())->getGameStateController()->loadNextMap();
	}

	if (o->hasProperty("destructible")) {
		if (entityProperties.getXvel() > 1500 || entityProperties.getXvel() < -1500) {
			o->destroy(data);
			return true;
		}
	}

	flat2d::EntityShape soldierBox = entityProperties.getColliderShape();
	flat2d::EntityShape objectCollider = o->getEntityProperties().getColliderShape();
	if (!motionController->reverseGravity && soldierBox.y + soldierBox.h < objectCollider.y) {
		grounded = true;
		doubleJumped = false;
	} else if (motionController->reverseGravity && soldierBox.y > objectCollider.y + objectCollider.h) {
		grounded = true;
		doubleJumped = false;
	}

	return false;
}

bool Soldier::handleRocketCollision(Rocket* o, const flat2d::GameData* data)
{
	if (spawnGraceTimer.isStarted()) {
		return true;
	}

	bool ghostMode = powerupContainer->isGhostMode();
	Rocket::Mode rocketMode = o->getMode();
	bool rocketGhostKiller = rocketMode == Rocket::Mode::GHOST || rocketMode == Rocket::Mode::MULTI;
	bool rocketNormalKiller = rocketMode == Rocket::Mode::NORMAL || rocketMode == Rocket::Mode::MULTI;

	if (ghostMode && rocketGhostKiller) {
		kill(data);
	} else if (!ghostMode && rocketNormalKiller) {
		kill(data);
	}

	return true;
}

void Soldier::kill(const flat2d::GameData *gameData)
{
	if (powerupContainer->isGhostMode()) {
		ghostEmitter->setEmissionCount(50);
	} else {
		bloodEmitter->setEmissionCount(200);
	}

	killed = true;
	entityProperties.setCollidable(false);
	entityProperties.setXvel(0);
	entityProperties.setYvel(0);
	fallTimer.stop();
	deathTimer.start();
	motionController->freeze();

	if (powerupContainer->isGhostMode()) {
		mixer->playEffect(Effects::SHATTER);
	} else {
		mixer->playEffect(Effects::SPLAT);
	}
}

void Soldier::restoreAtCheckpoint()
{
	killed = false;
	entityProperties.setCollidable(true);
	entityProperties.setXpos(checkPointX);
	entityProperties.setYpos(checkPointY);
	entityProperties.setXvel(0);
	entityProperties.setYvel(0);
	spawnGraceTimer.start();
}

SoldierPowerupContainer* Soldier::getPowerupContainer()
{
	return powerupContainer;
}
