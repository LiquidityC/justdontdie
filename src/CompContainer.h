#ifndef _COMP_CONTAINER_H
#define _COMP_CONTAINER_H

#include "objectcontainer.h"

class CompContainer
{
	private:
		ObjectContainer objectContainer;

	private:

		// Don't implement, singleton
		CompContainer() : objectContainer() { };
		CompContainer(const CompContainer&);
		CompContainer& operator=(const CompContainer&);

	public:

		static CompContainer& getInstance()
		{
			static CompContainer instance;
			return instance;
		}

		ObjectContainer& getObjectContainer() { return objectContainer; };
};

#endif
