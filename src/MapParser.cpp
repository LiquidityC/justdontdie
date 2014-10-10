#include <iostream>
#include <cstdlib>

#include "MapParser.h"
#include "MapTileObject.h"
#include "ResourceContainer.h"
#include "Layers.h"

using namespace rapidxml;
using namespace std;

bool MapParser::createMapFrom(ResourceContainer *resourceContainer, std::string dir, std::string filename,
		flat2d::RenderData *renderData)
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

	
	renderData->getCamera()->setMapDimensions(map.width * map.tileWidth, map.height * map.tileHeight);
	flat2d::ObjectContainer *objectContainer = renderData->getObjectContainer();


	// Parse all the layers (might need to add layers in ObjectContainer)
	while ( node && strcmp(node->name(), "layer") == 0) {
		xml_node<> *data = node->first_node();

		int row = 0;
		int col = 0;
		for (xml_node<> *tileNode = data->first_node(); tileNode; tileNode = tileNode->next_sibling()) {
			xml_attribute<> *gidAttr = tileNode->first_attribute();
			int gid = atoi(gidAttr->value());
			if (gid == 0) {
				col++;
				if (col >= map.width) {
					col = 0;
					row++;
				}
				continue;
			}

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
				SDL_Texture* texture = flat2d::MediaUtil::loadTexture(dir + tileset->sourcePath, renderData->getRenderer());
				tileset->texture = texture;
				resourceContainer->addTexture(texture);
			}

			if (tileset->tiles.find(gid) == tileset->tiles.end()) {
				Tile newTile;
				newTile.id = gid - 1;
				tileset->tiles[gid] = newTile;
			} 

			Tile *tile = &(tileset->tiles[gid]);

			// Create til obj
			MapTileObject* tileObj = new MapTileObject(col * map.tileWidth, row * map.tileHeight, 
					tileset->tileWidth, tileset->tileHeight, tileset->texture);

			// Set properties
			for (auto it = tile->properties.begin(); it != tile->properties.end(); it++) {
				tileObj->setProperty(it->first, it->second);
			}
			tileObj->setColliderBox(tile->collider);
			if (tile->collider.w != 0 && tile->collider.h != 0) {
				tileObj->setCollidable(true);
			}

			int xoffset = tile->id * tileset->tileWidth;
			int xclip = xoffset % tileset->width;
			int yclip = tileset->tileHeight * ((xoffset - xclip) / tileset->width);

			SDL_Rect clip = { xclip, yclip, tileset->tileWidth, tileset->tileHeight };
			tileObj->setClip(clip);
			objectContainer->registerObject(tileObj, Layers::BACK);

			col++;
			if (col >= map.width) {
				col = 0;
				row++;
			}
		}
		node = node->next_sibling();
	}


	while ( node && strcmp(node->name(), "objectgroup") == 0) {
		xml_node<> *object = node->first_node();
		while (object) {
			if ( strcmp(object->name(), "object") != 0) {
				continue;
			}

			SDL_Rect objBox;
			xml_attribute<> *attr = object->first_attribute()->next_attribute();
			objBox.x = static_cast<int>(atof(attr->value()));
			attr = attr->next_attribute();
			objBox.y = static_cast<int>(atof(attr->value()));
			attr = attr->next_attribute();
			objBox.w = static_cast<int>(atof(attr->value()));
			attr = attr->next_attribute();
			objBox.h = static_cast<int>(atof(attr->value()));

			MapTileObject* tileObj = new MapTileObject(objBox.x, objBox.y, objBox.w, objBox.h, NULL);
			objectContainer->registerObject(tileObj);

			xml_node<> *properties = object->first_node();
			object = object->next_sibling();
			if (!properties) {
				continue;
			}

			xml_node<> *property = properties->first_node();
			while (property) {
				xml_attribute<> *nameAttr = property->first_attribute();
				xml_attribute<> *valueAttr = nameAttr->next_attribute();
				tileObj->setProperty(nameAttr->value(), strcmp(valueAttr->value(), "true") == 0);
				property = property->next_sibling();
			}
		}
		node = node->next_sibling();
	}

	return true;
}

bool MapParser::parseTileset(xml_node<> *node)
{
	if ( strcmp(node->name(), "tileset") != 0) {
		cerr << "Node not named 'tileset' parse failed: '" << node->name() << "'" << endl;
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
		cerr << "Node not named 'image' parse failed: '" << node->name() << "'" << endl;
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

	// Skip terrain tags
	xml_node<> *firstTileNode = imageNode->next_sibling();
	while ( strcmp(firstTileNode->name(), "tile") != 0) {
		firstTileNode = firstTileNode->next_sibling();
	}

	int gid = tileset.firstgid;
	for (xml_node<> *tileNode = firstTileNode; tileNode; tileNode = tileNode->next_sibling()) {
		if ( strcmp(tileNode->name(), "tile") != 0) {
			cerr << "Node not named 'tile' parse failed: '" << node->name() << "'" << endl;
			return false;
		}

		int tileId = atoi(tileNode->first_attribute()->value());
		using namespace std;
		while (gid <= tileId) {
			Tile emptyTile;
			emptyTile.id = gid - 1;
			tileset.tiles[gid] = emptyTile;
			gid++;
		}

		Tile tile;
		tile.id = tileId;
		xml_node<> *firstNode = tileNode->first_node();
		if ( strcmp(firstNode->name(), "properties") == 0 ) {
			parseTileProperties(tile, firstNode);
			parseTileObjects(tile, firstNode->next_sibling());
		} else {
			parseTileObjects(tile, firstNode);
		}

		tileset.tiles[gid] = tile;
		gid++;
	}

	map.tilesets[tileset.firstgid] = tileset;

	return true;
}

void MapParser::parseTileProperties(Tile& tile, xml_node<> *properties)
{
	if (!properties) {
		return;
	}

	xml_node<> *property = properties->first_node();
	while (property) {
		xml_attribute<> *nameAttr = property->first_attribute();
		xml_attribute<> *valueAttr = nameAttr->next_attribute();
		tile.properties[nameAttr->value()] = strcmp( valueAttr->value(), "true") == 0;
		property = property->next_sibling();
	}
}

void MapParser::parseTileObjects(Tile& tile, xml_node<> *objectgroup)
{
	if (!objectgroup) {
		return;
	}

	xml_node<> *object = objectgroup->first_node();
	while (object) {
		if ( strcmp(object->name(), "object") != 0) {
			continue;
		}

		xml_attribute<> *attr = object->first_attribute();
		tile.collider.x = static_cast<int>(atof(attr->value()));
		attr = attr->next_attribute();
		tile.collider.y = static_cast<int>(atof(attr->value()));
		attr = attr->next_attribute();
		tile.collider.w = static_cast<int>(atof(attr->value()));
		attr = attr->next_attribute();
		tile.collider.h = static_cast<int>(atof(attr->value()));

		object = object->next_sibling();
	}
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
