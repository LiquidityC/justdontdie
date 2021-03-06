#ifndef SOLDIER_H_
#define SOLDIER_H_

#include <flat/flat.h>
#include <map>
#include <string>
#include "EntityType.h"
#include "GhostOverlay.h"
#include "SoldierMotionController.h"
#include "SoldierPowerupContainer.h"
#include "ParticleEmitter.h"

class MapTileObject;
class Rocket;

class Soldier : public flat2d::Entity
{
	friend class SoldierMotionController;
	friend class SoldierPowerupContainer;

	using ParticleEmitterList = std::map<std::string, ParticleEmitter*>;

	typedef struct {
		int x, y;
	} Clip;

	private:
		const std::string BLOOD_EMITTER		= "bloodEmitter";
		const std::string GHOST_EMITTER		= "ghostEmitter";
		const std::string CHANGE_EMITTER	= "changeEmitter";
		const std::string BOOST_EMITTER		= "boostEmitter";

		SoldierMotionController *motionController = nullptr;
		SoldierPowerupContainer *powerupContainer = nullptr;

		bool grounded = true;
		flat2d::Mixer *mixer = nullptr;

		bool killed = false;
		bool doubleJumped = true;
		bool floating = false;

		ParticleEmitterList particleEmitters;

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
				powerupContainer = new SoldierPowerupContainer(this);
				entityProperties.setColliderShape({ 5, 1, 18, 22 });
				particleEmitters[BOOST_EMITTER] = new ParticleEmitter(ParticleType::BOOST_PARTICLE);
				particleEmitters[CHANGE_EMITTER] = new ParticleEmitter(ParticleType::FIRE_PARTICLE);
				particleEmitters[BLOOD_EMITTER] = new ParticleEmitter(ParticleType::BLOOD_PARTICLE);
				particleEmitters[GHOST_EMITTER] = new ParticleEmitter(ParticleType::GHOST_PARTICLE);
				setInputHandler(true);
			}

		~Soldier() {
			for (auto &emitter : particleEmitters) {
				delete emitter.second;
			}
			delete motionController;
			delete powerupContainer;
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
