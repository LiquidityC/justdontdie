#include <iostream>
#include <algorithm>
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
	SDL_Rect clip = { 0, 0, locationProperty.getWidth(), locationProperty.getHeight() };
	setClip(clip);
	mixer = gameData->getMixer();

	ghostOverlay = new GhostOverlay();
	gameData->getObjectContainer()->registerObject(ghostOverlay, Layers::OVERLAY);
}

void Soldier::handle(const SDL_Event& e)
{
	if (killed) {
		return;
	}

	if (e.type != SDL_KEYDOWN) {
		return;
	}

	switch (e.key.keysym.sym) {
		case SDLK_SPACE:
		case SDLK_h:
		case SDLK_k:
			if( grounded || (!ghostMode && !doubleJumped) ) {
				yvel = -1050;
				doubleJumped = grounded ? false : true;
				grounded = false;
				mixer->playEffect(Effects::JUMP);
			}
			break;
		case SDLK_j:
			ghostMode = !ghostMode;
			ghostOverlay->setVisible(ghostMode);
			break;
		default:
			break;
	}
}

void Soldier::postHandle(const flat2d::GameData *gameData)
{
	xvel = 0;

	const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
	if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
		xvel = -300;
		facingLeft = true;
	}
	if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
		xvel = 300;
		facingLeft = false;
	}
	if (currentKeyStates[SDL_SCANCODE_K] && ghostMode && !grounded && yvel > 5) {
		yvel = 5;
	}

	if (spawnGraceTimer.isStarted() && spawnGraceTimer.getTicks() > 1000) {
		spawnGraceTimer.stop();
	}
}

void Soldier::preRender(const flat2d::GameData *data)
{
	if (killed && deathTimer.getTicks() < 3000) {
		return;
	} else if (killed) {
		restoreAtCheckpoint();
	}

	flat2d::CollisionDetector* colDetector = data->getCollisionDetector();
	float deltaTime = data->getDeltatimeMonitor()->getDeltaTime();

	// Gravity
	if (yvel < 800) {
		yvel += std::min(3600 * deltaTime, 800 - yvel);
	}

	// Try to move object vertically
	locationProperty.incrementXpos(xvel * deltaTime);
	GameObject *object;
	if ((object = colDetector->checkForCollisions(this)) != nullptr) {
		handleHorizontalCollision(object, data);
	}
	object = nullptr;

	// Try to move object horizontally
	locationProperty.incrementYpos(yvel * deltaTime);
	if ((object = colDetector->checkForCollisions(this)) != nullptr) {
		handleVerticalCollision(object, data);
	}

	calculateCurrentClip();

	data->getRenderData()->getCamera()->centerOn(locationProperty.getXpos() + (locationProperty.getWidth()/2),
			locationProperty.getYpos() + (locationProperty.getHeight()/2));
}

void Soldier::render(const flat2d::RenderData* data) const
{
	if (killed) {
		return;
	}

#ifdef DEBUG
	SDL_SetRenderDrawColor(data->getRenderer(), 0xFF, 0x00, 0x00, 0xFF );
	const flat2d::LocationProperty::Parents parents = locationProperty.getParents();
	for(auto it = parents.begin(); it != parents.end(); it++) {
		SDL_Rect bounds = (*it).getBoundingBox();
		bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
		bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
		SDL_RenderDrawRect( data->getRenderer(), &bounds );
	}
#endif

	RenderedGameObject::render(data);
}

void Soldier::calculateCurrentClip()
{
	static unsigned int frameSwitch = 0;
	static bool clipSwitch = false;

	int x = 0;
	int y = 0;

	if (ghostMode) {
		y = locationProperty.getHeight();
	}
	if (facingLeft) {
		x = 2 * locationProperty.getWidth();
	}

	if (xvel < 0) {
		x = 2 * locationProperty.getWidth();
	} else if (xvel > 0) {
		x = 0;
	}

	if (xvel != 0) {
		frameSwitch++;
		if (frameSwitch >= 4) {
			clipSwitch = !clipSwitch;
			frameSwitch = 0;
		}
		if (grounded) {
			x += clipSwitch ? locationProperty.getWidth() : 0;
		} else {
			x += locationProperty.getWidth();
		}
	}

	SDL_Rect clip = { x, y, locationProperty.getWidth(), locationProperty.getHeight() };
	setClip(clip);
}

bool Soldier::handleVerticalCollision(flat2d::GameObject *o, const flat2d::GameData* data)
{
	switch (o->getType()) {
		case GameObjectType::TILE:
			return handleVerticalTileCollision(static_cast<MapTileObject*>(o), data);
		default:
			return handleGeneralCollision(o, data);
	}
}

bool Soldier::handleHorizontalCollision(flat2d::GameObject *o, const flat2d::GameData* data)
{
	switch (o->getType()) {
		case GameObjectType::TILE:
			return handleHorizontalTileCollision(static_cast<MapTileObject*>(o), data);
		default:
			return handleGeneralCollision(o, data);
	}
}

bool Soldier::handleGeneralCollision(flat2d::GameObject *o, const flat2d::GameData* data)
{
	switch (o->getType()) {
		case GameObjectType::TILE:
			return handleGeneralTileCollision(static_cast<MapTileObject*>(o), data);
			break;
		case GameObjectType::ROCKET:
			return handleRocketCollision(static_cast<Rocket*>(o), data);
			break;
		default:
			break;
	}
	return false;
}

bool Soldier::handleVerticalTileCollision(MapTileObject *o, const flat2d::GameData* data)
{
	if (handleGeneralTileCollision(o, data)) {
		return true;
	}

	// Completly reach the obstruction if it's a tile
	int o_ypos = o->getLocationProperty().getYpos();
	int o_height = o->getLocationProperty().getHeight();
	if (yvel > 0) {
		locationProperty.setYpos(o_ypos - locationProperty.getHeight() - 1);
	} else {
		locationProperty.setYpos(o_ypos + o_height + 1);
	}

	if (yvel > 0) {
		grounded = true;
		doubleJumped = false;
	}
	yvel = 0;

	return true;
}

bool Soldier::handleHorizontalTileCollision(MapTileObject *o, const flat2d::GameData* data)
{
	if (handleGeneralTileCollision(o, data)) {
		return true;
	}

	// Completly reach the obstruction if it's a tile
	int o_xpos = o->getLocationProperty().getXpos();
	int o_width = o->getLocationProperty().getWidth();
	if (xvel > 0) {
		locationProperty.setXpos(o_xpos - locationProperty.getWidth() - 1);
	} else {
		locationProperty.setXpos(o_xpos + o_width + 1);
	}

	xvel = 0;

	return true;
}

bool Soldier::handleGeneralTileCollision(MapTileObject *o, const flat2d::GameData* data)
{
	if (o->hasProperty("deadly")) {

		if (ghostMode) {
			static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createGhostSprayAt(
					locationProperty.getXpos() + static_cast<int>(locationProperty.getWidth()/2),
					locationProperty.getYpos() + static_cast<int>(locationProperty.getHeight()/2));
		} else {
			static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createBloodSprayAt(
					locationProperty.getXpos() + static_cast<int>(locationProperty.getWidth()/2),
					locationProperty.getYpos() + static_cast<int>(locationProperty.getHeight()/2));
		}
		wasKilled();
		return true;
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
		static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createGhostSprayAt(
				locationProperty.getXpos() + static_cast<int>(locationProperty.getWidth()/2),
				locationProperty.getYpos() + static_cast<int>(locationProperty.getHeight()/2));

		SDL_Rect rocketBox = o->getBoundingBox();
		static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createExplosionAt(
				rocketBox.x + static_cast<int>(rocketBox.w/2),
				rocketBox.y + static_cast<int>(rocketBox.h/2));
		o->setDead(true);
		mixer->playEffect(Effects::BANG);
		wasKilled();
	} else if (!ghostMode && (rocketMode == Rocket::Mode::NORMAL || rocketMode == Rocket::Mode::MULTI)) {
		static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createBloodSprayAt(
				locationProperty.getXpos() + static_cast<int>(locationProperty.getWidth()/2),
				locationProperty.getYpos() + static_cast<int>(locationProperty.getHeight()/2));

		SDL_Rect rocketBox = o->getBoundingBox();
		static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createExplosionAt(
				rocketBox.x + static_cast<int>(rocketBox.w/2),
				rocketBox.y + static_cast<int>(rocketBox.h/2));
		o->setDead(true);
		mixer->playEffect(Effects::BANG);
		wasKilled();
	}


	return true;
}

void Soldier::wasKilled()
{
	killed = true;
	deathTimer.start();

	if (ghostMode) {
		mixer->playEffect(Effects::SHATTER);
	} else {
		mixer->playEffect(Effects::SPLAT);
	}
}

void Soldier::restoreAtCheckpoint()
{
	killed = false;
	locationProperty.setXpos(checkPointX);
	locationProperty.setYpos(checkPointY);
	xvel = 0;
	yvel = 0;
	spawnGraceTimer.start();
}

void Soldier::setGhostMode(bool ghostMode)
{
	this->ghostMode = ghostMode;
}
