#include <iostream>
#include <algorithm>
#include <cmath>
#include "Soldier.h"
#include "BloodParticle.h"
#include "LayerService.h"
#include "MapTileObject.h"
#include "Rocket.h"
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

	LayerService *layerService = static_cast<CustomGameData*>(gameData->getCustomGameData())->getLayerService();

	ghostOverlay = new GhostOverlay();
	gameData->getEntityContainer()->registerObject(ghostOverlay, layerService->getLayerIndex(OVERLAY_LAYER));
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
	return entityProperties.getYvel() > 60;
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
	if (killed && deathTimer.getTicks() < 3000) {
		return;
	} else if (killed) {
		restoreAtCheckpoint();
	}

	motionController->preMove(data);

	if (grounded) {
		grounded = !isFalling();
	}
	if (killedFromFalling(data)) {
		return;
	}

	// Gravity
	float yvel = entityProperties.getYvel();
	if (yvel < 800) {
		entityProperties.setYvel(yvel + std::min(60, 800 - static_cast<int>(yvel)));
	}
	if (floating && yvel > 100) {
		entityProperties.setYvel(100);
	}

	data->getRenderData()->getCamera()->centerOn(entityProperties.getXpos() + (entityProperties.getWidth()/2),
			entityProperties.getYpos() + (entityProperties.getHeight()/2));
	calculateCurrentClip();
}

void Soldier::render(const flat2d::RenderData* data) const
{
	if (killed) {
		return;
	}

#ifdef DEBUG
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

	if (ghostMode) {
		y = entityProperties.getHeight();
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
			break;
		case EntityType::ROCKET:
			return handleRocketCollision(static_cast<Rocket*>(o), data);
			break;
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
		setGhostMode(true);
		return true;
	}

	flat2d::EntityShape soldierBox = entityProperties.getColliderShape();
	if (soldierBox.y + soldierBox.h < o->getEntityProperties().getColliderShape().y) {
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

	Rocket::Mode rocketMode = o->getMode();
	if (ghostMode && (rocketMode == Rocket::Mode::GHOST || rocketMode == Rocket::Mode::MULTI)) {
		kill(data);
	} else if (!ghostMode && (rocketMode == Rocket::Mode::NORMAL || rocketMode == Rocket::Mode::MULTI)) {
		kill(data);
	}

	return true;
}

void Soldier::kill(const flat2d::GameData *gameData)
{
	ParticleEngine *particleEngine = static_cast<CustomGameData*>(
			gameData->getCustomGameData())->getParticleEngine();

	if (ghostMode) {
		particleEngine->createGhostSprayAt(
				entityProperties.getXpos() + static_cast<int>(entityProperties.getWidth()/2),
				entityProperties.getYpos() + static_cast<int>(entityProperties.getHeight()/2));
	} else {
		particleEngine->createBloodSprayAt(
				entityProperties.getXpos() + static_cast<int>(entityProperties.getWidth()/2),
				entityProperties.getYpos() + static_cast<int>(entityProperties.getHeight()/2));
	}

	killed = true;
	entityProperties.setCollidable(false);
	fallTimer.stop();
	deathTimer.start();
	motionController->freeze();

	if (ghostMode) {
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

void Soldier::setGhostMode(bool ghostMode)
{
	this->ghostMode = ghostMode;
	ghostOverlay->setVisible(ghostMode);
}

bool Soldier::isGhostMode() const
{
	return ghostMode;
}
