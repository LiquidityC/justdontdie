#include <iostream>
#include <algorithm>
#include "Soldier.h"
#include "BloodParticle.h"
#include "Layers.h"
#include "MapTileObject.h"
#include "Rocket.h"
#include "ParticleEngine.h"

void Soldier::init(const flat2d::RenderData *data)
{
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/soldier.png", data->getRenderer()));
	SDL_Rect clip = { 0, 0, width, height };
	setClip(clip);
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
			}
			break;
		case SDLK_j:
			ghostMode = !ghostMode;
			break;
		case SDLK_u:
			particleEngine->createGhostSprayAt(xpos, ypos);
			break;
		default:
			break;
	}
}

void Soldier::postHandle(const flat2d::GameData *gameData)
{
	xvel = 0;

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
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

void Soldier::preRender(const flat2d::RenderData *data)
{
	if (killed && deathTimer.getTicks() < 3000) {
		return;
	} else if (killed) {
		restoreAtCheckpoint();
	}

	flat2d::CollisionDetector* colDetector = data->getCollisionDetector();
	float deltaTime = data->getCamera()->getDeltaTime();

	// Gravity
	if (yvel < 800) {
		yvel += std::min(3600 * deltaTime, 800 - yvel);
	}

	xpos += (xvel * deltaTime);
	GameObject *object;
	if ((object = colDetector->checkForCollisions(this)) != NULL && !handleCollision(object, data)) {
		xpos -= (xvel * deltaTime);

		// Completly reach the obstruction
		while (!colDetector->hasCollided(this, object)) {
			xpos += xvel > 0 ? 1 : -1;
		}
		xpos += xvel > 0 ? -1 : 1;
		xvel = 0;
		object = NULL;
	}

	ypos += (yvel * deltaTime);
	if ((object = colDetector->checkForCollisions(this)) != NULL && !handleCollision(object, data)) {
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
		yvel = 0;
		object = NULL;
	}

	calculateCurrentClip();

	data->getCamera()->centerOn(xpos + (width/2), ypos + (height/2));
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
		y = 24;
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
		x += clipSwitch ? width : 0;
	}

	SDL_Rect clip = { x, y, width, height };
	setClip(clip);
}

bool Soldier::handleCollision(flat2d::GameObject *o, const flat2d::RenderData* data)
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

bool Soldier::handleTileCollision(MapTileObject *o, const flat2d::RenderData* data)
{
	if (o->hasProperty("deadly")) {

		if (ghostMode) {
			particleEngine->createGhostSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));
		} else {
			particleEngine->createBloodSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));
		}
		wasKilled();
		return true;
	}
	return false;
}

bool Soldier::handleRocketCollision(Rocket* o, const flat2d::RenderData* data)
{
	if (spawnGraceTimer.isStarted()) {
		return true;
	}
	if (o->isGhost() && ghostMode) {
		particleEngine->createGhostSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));

		SDL_Rect rocketBox = o->getBoundingBox();
		particleEngine->createExplosionAt(rocketBox.x + static_cast<int>(rocketBox.w/2),
				rocketBox.y + static_cast<int>(rocketBox.h/2));
		o->setDead(true);
		wasKilled();
	} else if (!o->isGhost() && !ghostMode) {
		particleEngine->createBloodSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));

		SDL_Rect rocketBox = o->getBoundingBox();
		particleEngine->createExplosionAt(rocketBox.x + static_cast<int>(rocketBox.w/2),
				rocketBox.y + static_cast<int>(rocketBox.h/2));
		o->setDead(true);
		wasKilled();
	}


	return true;
}

void Soldier::wasKilled()
{
	killed = true;
	deathTimer.start();
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
