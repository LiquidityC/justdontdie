#ifndef NPCS_AI_NONFALLINGWALKINGAGENT_H_
#define	NPCS_AI_NONFALLINGWALKINGAGENT_H_

#include "WalkingAgent.h"

class NonFallingWalkingAgent : public WalkingAgent
{
	public:
		explicit NonFallingWalkingAgent(int xv) : WalkingAgent(xv) { }

		virtual bool onHorizontalCollision(flat2d::Entity *collider, flat2d::Entity *entity, const flat2d::GameData *data);
};

#endif // NPCS_AI_NONFALLINGWALKINGAGENT_H_
