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
			flat2d::Entity(x, y, w, h), aiAgent(agent) {
				entityProperties.setCollidable(true);
			}

		~Enemy() {
			if (aiAgent != nullptr) {
				delete aiAgent;
			}
		}

		// TODO(Linus): This is a temp override, should probably be dropped
		// when we have an actual texture
		void render(const flat2d::RenderData*) const;

		void init(const flat2d::GameData *data);
		bool onCollision(flat2d::Entity *collider, const flat2d::GameData *data);
		bool onVerticalCollision(flat2d::Entity *collider, const flat2d::GameData *data);
		bool onHorizontalCollision(flat2d::Entity *collider, const flat2d::GameData *data);
		void preMove(const flat2d::GameData*);

		int getType() const {
			return EntityType::ENEMY;
		}
};

#endif // NPCS_ENEMY_H_
