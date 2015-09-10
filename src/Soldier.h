#ifndef SOLDIER_H_
#define SOLDIER_H_

#include <flat/flat.h>
#include "EntityType.h"
#include "GhostOverlay.h"
#include "SoldierMotionController.h"
#include "SoldierPowerupContainer.h"

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
		SoldierPowerupContainer powerupContainer;

		bool grounded = true;
		flat2d::Mixer *mixer = nullptr;

		bool killed = false;
		bool doubleJumped = true;
		bool floating = false;

		flat2d::Timer deathTimer;
		flat2d::Timer spawnGraceTimer;
		flat2d::Timer fallTimer;

		int checkPointX, checkPointY;
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

		bool isFalling() const;
		bool killedFromFalling(const flat2d::GameData*);

		void calculateCurrentClip();

		bool handleGeneralCollision(flat2d::Entity*, const flat2d::GameData*);
		bool handleGeneralTileCollision(MapTileObject*, const flat2d::GameData*);
		bool handleRocketCollision(Rocket*, const flat2d::GameData*);

		void kill(const flat2d::GameData*);
		void restoreAtCheckpoint();

	public:
		Soldier(unsigned int x, unsigned int y) :
			Entity(x, y, 28, 24),
			checkPointX(x),
			checkPointY(y) {
				motionController = new SoldierMotionController(this);
				entityProperties.setColliderShape({ 5, 5, 18, 18 });
			}

		~Soldier() {
			if (texture != nullptr) {
				SDL_DestroyTexture(texture);
				texture = nullptr;
			}
			delete motionController;
		}

		int getType() const {
			return EntityType::SOLDIER;
		}

		void init(const flat2d::GameData*);

		void handle(const SDL_Event& event);
		void postHandle(const flat2d::GameData*);

		void preMove(const flat2d::GameData*);
		void render(const flat2d::RenderData*) const;

		SoldierPowerupContainer* getPowerupContainer();

		bool onCollision(Entity *collider, const flat2d::GameData*);
};

#endif // SOLDIER_H_
