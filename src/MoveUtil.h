#ifndef MOVEUTIL_H_
#define	MOVEUTIL_H_

#include <flat/flat.h>

class MoveUtil
{
	public:
		static void applyGravity(flat2d::EntityProperties *entityProperties);
		static void applyFloat(flat2d::EntityProperties *entityProperties);
};

#endif // MOVEUTIL_H_
