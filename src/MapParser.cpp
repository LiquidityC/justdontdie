#include <iostream>
#include <cstdlib>

#include "MapParser.h"

using namespace rapidxml;
using namespace std;

bool MapParser::createMapFrom(std::string dir, std::string filename, flat2d::ObjectContainer& objectContainer)
{

	file<> xmlFile((dir + filename).c_str());
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	xml_node<> *node = doc.first_node();
	if ( strcmp(node->name(), "map") != 0) {
		cout << "Root node not named 'map' parse failed: '" << node->name() << "'" << endl;
		return false;
	}

	// Get map data
	if (!parseMapAttributes(node)) {
		return false;
	}

	// Get tileset data
	for (xml_node<> *tilesetNode = node->first_node(); tilesetNode; tilesetNode = tilesetNode->next_sibling()) {
		if ( strcmp(tilesetNode->name(), "tileset") != 0) {
			break;
		}
		if (!parseTileset(tilesetNode)) {
			return false;
		}
	}

	// Find the Layer node
	while ( strcmp(node->name(), "layer") != 0 && node ) {
		node = node->next_sibling();
	}

	if ( !node ) {
		return false;
	}

	
	// Parse all the layers (might need to add layers in ObjectContainer)
	while ( strcmp(node->name(), "layer") == 0) {
		xml_node<> *data = node->first_node();
		for (xml_node<> *tile = data->first_node(); tile; tile = tile->next_sibling()) {
			// TODO:
			// xml_attribute<> *gid = tile->first_attribute();
			//
			// Create a generic game object with the given Texture and clip data
			// Need to create a generic GamObject that takes a texture and size params.
			// Also need a ResourceContainer that handles deletion of all textures used in the map
			// Generic GameObjects can't handle deletion of textures since they are shared.
		}
		node = node->next_sibling();
	}

	return true;
}

bool MapParser::parseTileset(xml_node<> *node)
{
	if ( strcmp(node->name(), "tileset") != 0) {
		cout << "Node not named 'tileset' parse failed: '" << node->name() << "'" << endl;
		return false;
	}

	Tileset tileset;
	for(xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
		if ( strcmp(attr->name(), "firstgid") == 0) {
			tileset.firstgid = atoi(attr->value());
		} else if ( strcmp(attr->name(), "name") == 0) {
			tileset.name = attr->value();
		} else if ( strcmp(attr->name(), "tilewidth") == 0) {
			tileset.tileWidth = atoi(attr->value());
		} else if ( strcmp(attr->name(), "tileheight") == 0) {
			tileset.tileHeight = atoi(attr->value());
		}
	}

	xml_node<> *imageNode = node->first_node();
	if ( strcmp(imageNode->name(), "image") != 0) {
		cout << "Node not named 'image' parse failed: '" << node->name() << "'" << endl;
		return false;
	}

	for(xml_attribute<> *attr = imageNode->first_attribute(); attr; attr = attr->next_attribute()) {
		if ( strcmp(attr->name(), "source") == 0) {
			tileset.sourcePath = attr->value();
		} else if ( strcmp(attr->name(), "width") == 0) {
			tileset.width = atoi(attr->value());
		} else if ( strcmp(attr->name(), "height") == 0) {
			tileset.height = atoi(attr->value());
		}
	}

	for (xml_node<> *tileNode = imageNode->next_sibling(); tileNode; tileNode = tileNode->next_sibling()) {
		Tile tile;
		if ( strcmp(tileNode->name(), "tile") != 0) {
			cout << "Node not named 'tile' parse failed: '" << node->name() << "'" << endl;
			return false;
		}

		tile.id = atoi(tileNode->first_attribute()->value());
		xml_node<> *property = tileNode->first_node()->first_node();
		xml_attribute<> *collidableAttr = property->first_attribute();
		tile.collidable = strcmp(collidableAttr->name(), "collidable") == 0 && strcmp(collidableAttr->value(), "true") == 0;

		tileset.tiles[tile.id] = tile;
	}

	map.tilesets[tileset.firstgid] = tileset;

	return true;
}

bool MapParser::parseMapAttributes(xml_node<> *node)
{
	for(xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
		if ( strcmp(attr->name(), "version") == 0) {
			map.version = attr->value();
		} else if ( strcmp(attr->name(), "orientation") == 0) {
			map.orientation = attr->value();
		} else if ( strcmp(attr->name(), "render0rder") == 0) {
			map.renderOrder = attr->value();
		} else if ( strcmp(attr->name(), "width") == 0) {
			map.width = atoi(attr->value());
		} else if ( strcmp(attr->name(), "height") == 0) {
			map.height = atoi(attr->value());
		} else if ( strcmp(attr->name(), "tilewidth") == 0) {
			map.tileWidth = atoi(attr->value());
		} else if ( strcmp(attr->name(), "tileheight") == 0) {
			map.tileHeight = atoi(attr->value());
		}
	}

	return true;
}
