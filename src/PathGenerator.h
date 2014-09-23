#ifndef _PATH_GENERATOR_H
#define _PATH_GENERATOR_H

#include <cstdio>

namespace flat2d
{
	class ObjectContainer;
}
class Block;

class PathGenerator
{
	private:
		flat2d::ObjectContainer* objectContainer;
		unsigned int pathWidth, pathGoalWidth, pathMaxWidth, turningRatio;
		Block* leftBlock;
		Block* rightBlock;

	public:
		PathGenerator(flat2d::ObjectContainer* objectContainer) : pathWidth(100), pathGoalWidth(200), turningRatio(0) {
			this->objectContainer = objectContainer;
			leftBlock = NULL;
			rightBlock = NULL;
		};

		~PathGenerator() { };

		void generatePath(int, int);

	private:
		PathGenerator(const PathGenerator&); // Don't implement, single instance
		const PathGenerator& operator=(const PathGenerator&); // Don't implement, one instance
};


#endif
