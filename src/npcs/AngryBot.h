#ifndef NPCS_ANGRYBOT_H_
#define	NPCS_ANGRYBOT_H_

#include <flat/flat.h>
#include "Enemy.h"
#include "ai/WalkingAgent.h"

class AngryBot : public Enemy
{
	public:
		AngryBot(int x, int y) : Enemy(new WalkingAgent(-300), x, y, 32, 32) { }
		void init(const flat2d::GameData*);
		void preMove(const flat2d::GameData*);
};

#endif // NPCS_ANGRYBOT_H_
