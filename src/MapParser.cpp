#include <iostream>
#include <cstdlib>

#include <flat/MediaUtil.h>

#include "MapParser.h"
#include "GenericGameObject.h"
#include "CompContainer.h"

using namespace rapidxml;
using namespace std;

bool MapParser::createMapFrom(std::string dir, std::string filename, SDL_Renderer* renderer)
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
	node = node->first_node();
	while ( node && strcmp(node->name(), "layer") != 0 ) {
		node = node->next_sibling();
	}

	if ( !node ) {
		cerr << "Failed to load layers" << endl;
		return false;
	}

	
	CompContainer::getInstance().getCamera().setMapDimensions(map.width * map.tileWidth, map.height * map.tileHeight);
	flat2d::ObjectContainer& objectContainer = CompContainer::getInstance().getObjectContainer();
	ResourceContainer& resourceContainer = CompContainer::getInstance().getResourceContainer();


	// Parse all the layers (might need to add layers in ObjectContainer)
	while ( node && strcmp(node->name(), "layer") == 0) {
		xml_node<> *data = node->first_node();

		int row = 0;
		int col = 0;
		for (xml_node<> *tileNode = data->first_node(); tileNode; tileNode = tileNode->next_sibling()) {
			xml_attribute<> *gidAttr = tileNode->first_attribute();
			int gid = atoi(gidAttr->value());

			Tileset *tileset = NULL;
			for (auto it = map.tilesets.begin(); it != map.tilesets.end(); it++) {
				if (it->first == gid) {
					tileset = &(it->second);
					break;
				} else if (it->first > gid) {
					break;
				}
				tileset = &(it->second);
			}

			if (tileset == NULL) {
				cerr << "Major parse fail" << endl;
				return false;
			}

			if (tileset->texture == NULL) {
				SDL_Texture* texture = flat2d::MediaUtil::loadTexture(dir + tileset->sourcePath, renderer);
				tileset->texture = texture;
				resourceContainer.addTexture(texture);
			}

			Tile *tile = &(tileset->tiles[gid]);
			if (!tile) {
				cerr << "Unable to locate correct tile" << endl;
				return false;
			}


			GenericGameObject* tileObj = new GenericGameObject(col * map.tileWidth, row * map.tileHeight, 
					tileset->tileWidth, tileset->tileHeight, tileset->texture);
			tileObj->setCollidable(tile->collidable);
			tileObj->setClipCoordinates((tile->id * tileset->tileWidth) % tileset->width, 0);
			objectContainer.registerObject(tileObj, Layers::BACK);

			col++;
			if (col >= map.width) {
				col = 0;
				row++;
			}
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
	tileset.texture = NULL;
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

	int gid = tileset.firstgid;
	for (xml_node<> *tileNode = imageNode->next_sibling(); tileNode; tileNode = tileNode->next_sibling()) {
		Tile tile;
		if ( strcmp(tileNode->name(), "tile") != 0) {
			cout << "Node not named 'tile' parse failed: '" << node->name() << "'" << endl;
			return false;
		}

		tile.id = atoi(tileNode->first_attribute()->value());
		xml_node<> *property = tileNode->first_node()->first_node();
		xml_attribute<> *nameAttr = property->first_attribute();
		xml_attribute<> *valueAttr = nameAttr->next_attribute();
		tile.collidable = strcmp(nameAttr->value(), "collidable") == 0 && strcmp(valueAttr->value(), "true") == 0;

		tileset.tiles[gid] = tile;
		gid++;
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
