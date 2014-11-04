#ifndef _SOLDIER_H
#define _SOLDIER_H

#include <flat/flat.h>
#include "GameObjectType.h"

class MapTileObject;
class Rocket;
class ParticleEngine;

class Soldier : public flat2d::RenderedGameObject
{
	typedef struct {
		int x, y;
	} Clip;

	private:
		ParticleEngine *particleEngine;
		bool grounded = false;

		float xvel = 0;
		float yvel = 0;

		bool killed = false;
		bool doubleJumped = true;

		flat2d::Timer deathTimer;
		flat2d::Timer spawnGraceTimer;

		int checkPointX, checkPointY;
		bool ghostMode = false;
		bool facingLeft = false;

		enum ClipIndex { RIGHT,
			LEFT,
			GHOST_RIGHT,
			GHOST_LEFT,
			WALK_RIGHT_1,
			WALK_RIGHT_2,
			WALK_LEFT_1,
			WALK_LEFT_2
		};

		SDL_Texture* soldierTexture = NULL;

		void calculateCurrentClip();

		bool handleCollision(flat2d::GameObject*, const flat2d::RenderData*);
		bool handleTileCollision(MapTileObject*, const flat2d::RenderData*);
		bool handleRocketCollision(Rocket*, const flat2d::RenderData*);

		void wasKilled();
		void restoreAtCheckpoint();

	public:

		Soldier(ParticleEngine *pe, unsigned int x, unsigned int y) :
			RenderedGameObject(x, y, 28, 24),
			particleEngine(pe),
			checkPointX(x),
			checkPointY(y) { };

		~Soldier() {
			if (texture != NULL) {
				SDL_DestroyTexture(soldierTexture);
				soldierTexture = NULL;
			}
		};

		int getType() {
			return GameObjectType::BOT;
		};

		void init(const flat2d::GameData*, const flat2d::RenderData*);

		void handle(const SDL_Event& event);
		void postHandle(const flat2d::GameData*);

		void preRender(const flat2d::RenderData*);
		void render(const flat2d::RenderData*) const;

		void setGhostMode(bool);
};

#endif
