#ifndef NPCS_AI_AGENT_H_
#define	NPCS_AI_AGENT_H_

#include <flat/flat.h>

class Agent
{
	public:
		Agent() { }

		virtual ~Agent() { }

		virtual bool onCollision(flat2d::Entity *collider,
				flat2d::Entity *entity,
				const flat2d::GameData *data) {  return false;}

		virtual bool onHorizontalCollision(flat2d::Entity *collider,
				flat2d::Entity *entity,
				const flat2d::GameData *data) {  return false;}

		virtual bool onVerticalCollision(flat2d::Entity *collider,
				flat2d::Entity *entity,
				const flat2d::GameData *data) {  return false;}

		virtual void handleEntity(flat2d::Entity *entity) { }
};

#endif // NPCS_AI_AGENT_H_
