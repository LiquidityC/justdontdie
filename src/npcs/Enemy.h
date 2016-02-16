#ifndef NPCS_ENEMY_H_
#define	NPCS_ENEMY_H_

#include <flat/flat.h>
#include "ai/Agent.h"
#include "../EntityType.h"

class Enemy : public flat2d::Entity
{
	private:
		Agent *aiAgent = nullptr;

	public:
		Enemy(Agent *agent, int x, int y, int w, int h) :
			flat2d::Entity(x, y, w, h), aiAgent(agent) { }

		virtual ~Enemy() {
			if (aiAgent != nullptr) {
				delete aiAgent;
			}
		}

		virtual void init(const flat2d::GameData*);
		virtual bool onCollision(flat2d::Entity *collider, const flat2d::GameData *data);
		virtual bool onVerticalCollision(flat2d::Entity *collider, const flat2d::GameData *data);
		virtual bool onHorizontalCollision(flat2d::Entity *collider, const flat2d::GameData *data);
		virtual void preMove(const flat2d::GameData*);

		int getType() const {
			return EntityType::ENEMY;
		}
};

#endif // NPCS_ENEMY_H_
