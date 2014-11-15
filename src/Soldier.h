#ifndef _SOLDIER_H
#define _SOLDIER_H

#include <flat/flat.h>
#include "GameObjectType.h"
#include "GhostOverlay.h"

class MapTileObject;
class Rocket;

class Soldier : public flat2d::RenderedGameObject
{
	typedef struct {
		int x, y;
	} Clip;

	private:
		bool grounded = false;
		flat2d::Mixer *mixer = nullptr;

		float xvel = 0;
		float yvel = 0;

		bool killed = false;
		bool doubleJumped = true;

		flat2d::Timer deathTimer;
		flat2d::Timer spawnGraceTimer;

		int checkPointX, checkPointY;
		bool ghostMode = false;
		bool facingLeft = false;

		GhostOverlay *ghostOverlay;

		enum ClipIndex { RIGHT,
			LEFT,
			GHOST_RIGHT,
			GHOST_LEFT,
			WALK_RIGHT_1,
			WALK_RIGHT_2,
			WALK_LEFT_1,
			WALK_LEFT_2
		};

		SDL_Texture* soldierTexture = nullptr;

		void calculateCurrentClip();

		bool handleCollision(flat2d::GameObject*, const flat2d::GameData*);
		bool handleTileCollision(MapTileObject*, const flat2d::GameData*);
		bool handleRocketCollision(Rocket*, const flat2d::GameData*);

		void wasKilled();
		void restoreAtCheckpoint();

	public:

		Soldier(unsigned int x, unsigned int y) :
			RenderedGameObject(x, y, 28, 24),
			checkPointX(x),
			checkPointY(y) { };

		~Soldier() {
			if (texture != nullptr) {
				SDL_DestroyTexture(soldierTexture);
				soldierTexture = nullptr;
			}
		};

		int getType() {
			return GameObjectType::BOT;
		};

		void init(const flat2d::GameData*, const flat2d::RenderData*);

		void handle(const SDL_Event& event);
		void postHandle(const flat2d::GameData*);

		void preRender(const flat2d::GameData*);
		void render(const flat2d::RenderData*) const;

		void setGhostMode(bool);
};

#endif
