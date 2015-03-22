#ifndef SOLDIER_H_
#define SOLDIER_H_

#include <flat/flat.h>
#include "EntityType.h"
#include "GhostOverlay.h"
#include "SoldierMotionController.h"

class MapTileObject;
class Rocket;

class Soldier : public flat2d::Entity
{
	friend class SoldierMotionController;
	typedef struct {
		int x, y;
	} Clip;

	private:
		SoldierMotionController *motionController = nullptr;
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

		bool handleVerticalCollision(flat2d::Entity*, const flat2d::GameData*);
		bool handleHorizontalCollision(flat2d::Entity*, const flat2d::GameData*);
		bool handleGeneralCollision(flat2d::Entity*, const flat2d::GameData*);
		bool handleVerticalTileCollision(MapTileObject*, const flat2d::GameData*);
		bool handleHorizontalTileCollision(MapTileObject*, const flat2d::GameData*);
		bool handleGeneralTileCollision(MapTileObject*, const flat2d::GameData*);
		bool handleRocketCollision(Rocket*, const flat2d::GameData*);

		void wasKilled();
		void restoreAtCheckpoint();

	public:
		Soldier(unsigned int x, unsigned int y) :
			Entity(x, y, 28, 24),
			checkPointX(x),
			checkPointY(y) {
				motionController = new SoldierMotionController(this);
			}

		~Soldier() {
			if (texture != nullptr) {
				SDL_DestroyTexture(soldierTexture);
				soldierTexture = nullptr;
				delete motionController;
			}
		}

		int getType() {
			return EntityType::BOT;
		}

		void init(const flat2d::GameData*);

		void handle(const SDL_Event& event);
		void postHandle(const flat2d::GameData*);

		void preRender(const flat2d::GameData*);
		void render(const flat2d::RenderData*) const;

		void setGhostMode(bool ghostMode);
};

#endif // SOLDIER_H_
