#include <iostream>
#include "WalkingAgent.h"

void WalkingAgent::handleEntity(flat2d::Entity *entity)
{
	flat2d::EntityProperties& props = entity->getEntityProperties();
	if (xvel != props.getXvel()) {
		props.setXvel(xvel);
	}
}

bool WalkingAgent::onHorizontalCollision(flat2d::Entity *collider, flat2d::Entity *entity, const flat2d::GameData *data)
{
	flat2d::EntityProperties &colliderProps = collider->getEntityProperties();
	flat2d::EntityProperties &props = entity->getEntityProperties();
	if (colliderProps.getXpos() < props.getXpos() && xvel < 0) {
		xvel = -1*xvel;
	} else if (colliderProps.getXpos() > props.getXpos() && xvel > 0) {
		xvel = -1*xvel;
	}
	return false;
}
