#ifndef MAP_ONESHOTENEMYGENERATOR_H_
#define	MAP_ONESHOTENEMYGENERATOR_H_

#include "EnemyGenerator.h"

class OneShotEnemyGenerator : public EnemyGenerator
{
	private:
		bool hasFired = false;

	protected:
		bool timeToSpawn() {
			if (!hasFired) {
				hasFired = true;
				return true;
			}
			return false;
		}
};


#endif // MAP_ONESHOTENEMYGENERATOR_H_
