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
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/soldier.png", gameData->getRenderData()->getRenderer()));
	SDL_Rect clip = { 0, 0, width, height };
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
			if( grounded || !doubleJumped ) {
				yvel = -1000;
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

	xpos += (xvel * deltaTime);
	GameObject *object;
	if ((object = colDetector->checkForCollisions(this)) != nullptr && !handleCollision(object, data)) {
		xpos -= (xvel * deltaTime);

		// Completly reach the obstruction
		while (!colDetector->hasCollided(this, object)) {
			xpos += xvel > 0 ? 1 : -1;
		}
		xpos += xvel > 0 ? -1 : 1;
		xvel = 0;
		object = nullptr;
	}

	ypos += (yvel * deltaTime);
	if ((object = colDetector->checkForCollisions(this)) != nullptr && !handleCollision(object, data)) {
		ypos -= (yvel * deltaTime);

		// Completly ground the soldier
		while (!colDetector->hasCollided(this, object)) {
			ypos += yvel > 0 ? 1 : -1;
		}
		ypos += yvel > 0 ? -1 : 1;
		if (yvel > 0) {
			grounded = true;
			doubleJumped = false;
		}
		object = nullptr;
		yvel = 0;
	}

	calculateCurrentClip();

	data->getRenderData()->getCamera()->centerOn(xpos + (width/2), ypos + (height/2));
}

void Soldier::render(const flat2d::RenderData* data) const
{
	if (killed) {
		return;
	}
	RenderedGameObject::render(data);
}

void Soldier::calculateCurrentClip()
{
	static unsigned int frameSwitch = 0;
	static bool clipSwitch = false;

	int x = 0;
	int y = 0;

	if (ghostMode) {
		y = height;
	}
	if (facingLeft) {
		x = 2 * width;
	}

	if (xvel < 0) {
		x = 2 * width;
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
			x += clipSwitch ? width : 0;
		} else {
			x += width;
		}
	}

	SDL_Rect clip = { x, y, width, height };
	setClip(clip);
}

bool Soldier::handleCollision(flat2d::GameObject *o, const flat2d::GameData* data)
{
	switch (o->getType()) {
		case GameObjectType::TILE:
			return handleTileCollision(static_cast<MapTileObject*>(o), data);
			break;
		case GameObjectType::ROCKET:
			return handleRocketCollision(static_cast<Rocket*>(o), data);
			break;
		default:
			break;
	}
	return false;
}

bool Soldier::handleTileCollision(MapTileObject *o, const flat2d::GameData* data)
{
	if (o->hasProperty("deadly")) {

		if (ghostMode) {
			static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createGhostSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));
		} else {
			static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createBloodSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));
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
				xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));

		SDL_Rect rocketBox = o->getBoundingBox();
		static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createExplosionAt(
				rocketBox.x + static_cast<int>(rocketBox.w/2),
				rocketBox.y + static_cast<int>(rocketBox.h/2));
		o->setDead(true);
		mixer->playEffect(Effects::JUMP);
		wasKilled();
	} else if (!ghostMode && (rocketMode == Rocket::Mode::NORMAL || rocketMode == Rocket::Mode::MULTI)) {
		static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createBloodSprayAt(
				xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));

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
	xpos = checkPointX;
	ypos = checkPointY;
	xvel = 0;
	yvel = 0;
	spawnGraceTimer.start();
}

void Soldier::setGhostMode(bool ghostMode)
{
	this->ghostMode = ghostMode;
}
