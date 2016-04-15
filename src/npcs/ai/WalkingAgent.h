#ifndef NPCS_AI_WALKINGAGENT_H_
#define	NPCS_AI_WALKINGAGENT_H_

#include "Agent.h"

class WalkingAgent : public Agent
{
	protected:
		int xvel;

	public:
		explicit WalkingAgent(int xv) : Agent(), xvel(xv) { }

		virtual bool onHorizontalCollision(flat2d::Entity *collider, flat2d::Entity *entity, const flat2d::GameData *data);
		virtual void handleEntity(flat2d::Entity *entity);
};

#endif // NPCS_AI_WALKINGAGENT_H_
