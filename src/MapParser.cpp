#include <iostream>

#include "MapParser.h"
#include "NewMacro.h"

void MapParser::createMapFrom(std::string path, flat2d::ObjectContainer& objectContainer)
{
	using namespace rapidxml;
	using namespace std;

	file<> xmlFile(path.c_str());
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	cout << "Name of my first node is: " << doc.first_node()->name() << endl;
}
