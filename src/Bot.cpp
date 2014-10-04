#include <iostream>
#include <algorithm>
#include "Bot.h"
#include "BloodParticle.h"
#include "Layers.h"
#include "MapTileObject.h"
#include "Rocket.h"
#include "ParticleEngine.h"

void Bot::init(const flat2d::RenderData *data)
{
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/bot.png", data->getRenderer()));
	setClip(clips[currentClip]);
}

void Bot::handle(const SDL_Event& e)
{
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
		default:
			break;
	}
}

void Bot::postHandle(const flat2d::GameData *gameData)
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

}

void Bot::preRender(const flat2d::RenderData *data)
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

		// Completly ground the bot
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

void Bot::render(const flat2d::RenderData* data) const
{
	if (killed) {
		return;
	}
	RenderedGameObject::render(data);
}

void Bot::calculateCurrentClip()
{
	if (xvel != 0) {
		if (xvel > 0
				&& currentClip != ClipIndex::RIGHT
				&& currentClip != ClipIndex::WALK_RIGHT_1
				&& currentClip != ClipIndex::WALK_RIGHT_2)
		{
			currentClip = ClipIndex::WALK_RIGHT_1;
		} else if (xvel < 0
				&& currentClip != ClipIndex::LEFT
				&& currentClip != ClipIndex::WALK_LEFT_1
				&& currentClip != ClipIndex::WALK_LEFT_2)
		{
			currentClip = ClipIndex::WALK_LEFT_1;
		}

		if (xvel > 0) {
			static int rightClipCount = 0;
			static int rightClipIndex = 0;

			ClipIndex walkRightClips[4] = { 
				ClipIndex::WALK_RIGHT_1,
				ClipIndex::RIGHT,
				ClipIndex::WALK_RIGHT_2,
				ClipIndex::RIGHT
			};

			if (rightClipCount > 2) {
				rightClipCount = 0;

				currentClip = walkRightClips[rightClipIndex];

				++rightClipIndex;
				if (rightClipIndex > 3) {
					rightClipIndex = 0;
				}
			}
			++rightClipCount;

		} else if (xvel < 0) {
			static int leftClipCount = 0;
			static int leftClipIndex = 0;

			ClipIndex walkLeftClips[4] = { 
				ClipIndex::WALK_LEFT_1,
				ClipIndex::LEFT,
				ClipIndex::WALK_LEFT_2,
				ClipIndex::LEFT
			};

			if (leftClipCount > 2) {
				leftClipCount = 0;

				currentClip = walkLeftClips[leftClipIndex];

				++leftClipIndex;
				if (leftClipIndex > 3) {
					leftClipIndex = 0;
				}
			}
			++leftClipCount;
		}
	}

	if (xvel == 0 && yvel == 0 && grounded) {
		if (facingLeft) {
			if (ghostMode) {
				currentClip = ClipIndex::GHOST_LEFT;
			} else {
				currentClip = ClipIndex::LEFT;
			}

		} else {
			if (ghostMode) {
				currentClip = ClipIndex::GHOST_RIGHT;
			} else {
				currentClip = ClipIndex::RIGHT;
			}
		}
	}

	setClip(clips[currentClip]);
}

bool Bot::handleCollision(flat2d::GameObject *o, const flat2d::RenderData* data)
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

bool Bot::handleTileCollision(MapTileObject *o, const flat2d::RenderData* data)
{
	if (o->hasProperty("deadly")) {
		particleEngine->createBloodSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));
		wasKilled();
		return true;
	}
	return false;
}

bool Bot::handleRocketCollision(Rocket* o, const flat2d::RenderData* data)
{
	if (o->isGhost() && ghostMode) {
		particleEngine->createBloodSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));
		wasKilled();
	} else if (!o->isGhost() && !ghostMode) {
		particleEngine->createBloodSprayAt(xpos + static_cast<int>(width/2), ypos + static_cast<int>(height/2));
		wasKilled();
	}


	return true;
}

void Bot::wasKilled()
{
	killed = true;
	deathTimer.start();
}

void Bot::restoreAtCheckpoint()
{
	killed = false;
	xpos = checkPointX;
	ypos = checkPointY;
	xvel = 0;
	yvel = 0;
}
