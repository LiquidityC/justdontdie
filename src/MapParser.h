#ifndef _MAP_PARSER_H
#define _MAP_PARSER_H

#include <flat/ObjectContainer.h>
#include <string>

class MapParser
{
	public:
		void createMapFrom(std::string, flat2d::ObjectContainer&);
};

#endif
