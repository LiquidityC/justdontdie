#include <iostream>
#include <cstdlib>
#include <string>

#include "MapParser.h"
#include "MapTileObject.h"
#include "CustomGameData.h"
#include "ResourceContainer.h"
#include "LayerService.h"

using rapidxml::file;
using rapidxml::xml_document;
using rapidxml::xml_node;
using rapidxml::xml_attribute;

bool MapParser::createMapFrom(flat2d::GameData *gameData, std::string dir, std::string filename)
{
	flat2d::RenderData *renderData = gameData->getRenderData();

	file<> xmlFile((dir + filename).c_str());
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	xml_node<> *node = doc.first_node();
	if ( !checkNodeName(node, "map") ) {
		std::cout << "Root node not named 'map' parse failed: '" << node->name() << "'" << std::endl;
		return false;
	}

	// Get map data
	if (!parseMapAttributes(node)) {
		return false;
	}

	// Get tileset data
	parseTilesets(node);

	// Find the Layer node and parse layers
	node = node->first_node();
	while ( node && !checkNodeName(node, "layer") ) {
		node = node->next_sibling();
	}
	if ( !node ) {
		std::cerr << "Failed to load layers" << std::endl;
		return false;
	}

	renderData->getCamera()->setMapDimensions(map.width * map.tileWidth, map.height * map.tileHeight);

	// Parse all the layers (might need to add layers in EntityContainer)
	while ( node && (checkNodeName(node, "layer") || checkNodeName(node, "objectgroup")) ) {
		bool success = false;
		if (checkNodeName(node, "layer")) {
			success = parseLayer(node, gameData, dir);
		} else if (checkNodeName(node, "objectgroup")) {
			success = parseObjectLayer(node, gameData);
		}

		if (!success) {
			std::cerr << "Failed to parse layers" << std::endl;
			return false;
		}

		node = node->next_sibling();
	}

	return true;
}

bool MapParser::parseTilesets(xml_node<> *node)
{
	for (xml_node<> *tilesetNode = node->first_node();
			tilesetNode;
			tilesetNode = tilesetNode->next_sibling())
	{
		if ( !checkNodeName(tilesetNode, "tileset") ) {
			break;
		}
		if (!parseTileset(tilesetNode)) {
			return false;
		}
	}

	return true;
}

bool MapParser::parseTileset(xml_node<> *node)
{
	if ( !checkNodeName(node, "tileset") ) {
		std::cerr << "Node not named 'tileset' parse failed: '" << node->name() << "'" << std::endl;
		return false;
	}

	Tileset tileset;
	tileset.texture = nullptr;
	for(xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
		if ( checkAttrName(attr, "firstgid") ) {
			tileset.firstgid = atoi(attr->value());
		} else if ( checkAttrName(attr, "name") ) {
			tileset.name = attr->value();
		} else if ( checkAttrName(attr, "tilewidth") ) {
			tileset.tileWidth = atoi(attr->value());
		} else if ( checkAttrName(attr, "tileheight") ) {
			tileset.tileHeight = atoi(attr->value());
		}
	}

	xml_node<> *imageNode = node->first_node();
	if ( !checkNodeName(imageNode, "image") ) {
		std::cerr << "Node not named 'image' parse failed: '" << node->name() << "'" << std::endl;
		return false;
	}

	for(xml_attribute<> *attr = imageNode->first_attribute(); attr; attr = attr->next_attribute()) {
		if ( checkAttrName(attr, "source") ) {
			tileset.sourcePath = attr->value();
		} else if ( checkAttrName(attr, "width") ) {
			tileset.width = atoi(attr->value());
		} else if ( checkAttrName(attr, "height") ) {
			tileset.height = atoi(attr->value());
		}
	}

	// Skip terrain tags
	xml_node<> *firstTileNode = imageNode->next_sibling();
	while ( firstTileNode && !checkNodeName(firstTileNode, "tile") ) {
		firstTileNode = firstTileNode->next_sibling();
	}

	if (!firstTileNode) {
		std::cerr << "Error, couldn't find tile node in tileset" << std::endl;
		return false;
	}

	int gid = tileset.firstgid;
	for (xml_node<> *tileNode = firstTileNode; tileNode; tileNode = tileNode->next_sibling()) {
		if ( !checkNodeName(tileNode, "tile") ) {
			std::cerr << "Node not named 'tile' parse failed: '" << node->name() << "'" << std::endl;
			return false;
		}

		int tileId = atoi(tileNode->first_attribute()->value());
		while (gid <= tileId) {
			Tile emptyTile;
			emptyTile.id = gid - 1;
			tileset.tiles[gid] = emptyTile;
			gid++;
		}

		Tile tile;
		tile.id = tileId;
		xml_node<> *firstNode = tileNode->first_node();
		if (firstNode) {
			if ( checkNodeName(firstNode, "properties") ) {
				parseTileProperties(tile, firstNode);
				parseTileObjects(tile, firstNode->next_sibling());
			} else {
				parseTileObjects(tile, firstNode);
			}
		}

		tileset.tiles[gid] = tile;
		gid++;
	}

	map.tilesets[tileset.firstgid] = tileset;

	return true;
}

bool MapParser::parseLayer(xml_node<> *node, flat2d::GameData *gameData, std::string dir)
{
	xml_node<> *data = node->first_node();

	CustomGameData *customGameData = static_cast<CustomGameData*>(gameData->getCustomGameData());
	LayerService *layerService = customGameData->getLayerService();
	ResourceContainer *resourceContainer = customGameData->getResourceContainer();
	flat2d::RenderData *renderData = gameData->getRenderData();

	// Extract layer name
	std::string layerName = getNameAttrValue(data);
	layerService->registerLayer(layerName);
	layerService->registerLayers(gameData->getEntityContainer());

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

		Tileset *tileset = nullptr;
		for (auto it = map.tilesets.begin(); it != map.tilesets.end(); it++) {
			if (it->first == gid) {
				tileset = &(it->second);
				break;
			} else if (it->first > gid) {
				break;
			}
			tileset = &(it->second);
		}

		if (tileset == nullptr) {
			std::cerr << "Major parse fail" << std::endl;
			return false;
		}

		if (tileset->texture == nullptr) {
			SDL_Texture* texture = flat2d::MediaUtil::loadTexture(
					dir + tileset->sourcePath, renderData->getRenderer());
			tileset->texture = texture;
			resourceContainer->addTexture(texture);
		}

		if (tileset->tiles.find(gid) == tileset->tiles.end()) {
			Tile newTile;
			newTile.id = gid - 1;
			tileset->tiles[gid] = newTile;
		}

		Tile *tile = &(tileset->tiles[gid]);

		// Create tile obj
		MapTileObject* tileObj = new MapTileObject(col * map.tileWidth, row * map.tileHeight,
				tileset->tileWidth, tileset->tileHeight, tileset->texture);

		// Set properties
		for (auto it = tile->properties.begin(); it != tile->properties.end(); it++) {
			tileObj->setProperty(it->first, it->second);
		}
		tileObj->getEntityProperties().setColliderShape(tile->collider);
		if (tile->collider.w != 0 && tile->collider.h != 0) {
			tileObj->getEntityProperties().setCollidable(true);
		}

		int xoffset = tile->id * tileset->tileWidth;
		int xclip = xoffset % tileset->width;
		int yclip = tileset->tileHeight * ((xoffset - xclip) / tileset->width);

		SDL_Rect clip = { xclip, yclip, tileset->tileWidth, tileset->tileHeight };
		tileObj->setClip(clip);
		gameData->getEntityContainer()->registerObject(tileObj, layerService->getLayerIndex(layerName));

		col++;
		if (col >= map.width) {
			col = 0;
			row++;
		}
	}

	return true;
}

bool MapParser::parseObjectLayer(rapidxml::xml_node<> *node, flat2d::GameData *gameData)
{
	xml_node<> *object = node->first_node();

	CustomGameData *customGameData = static_cast<CustomGameData*>(gameData->getCustomGameData());
	LayerService *layerService = customGameData->getLayerService();

	// Get the layer name
	std::string layerName = getNameAttrValue(object);
	layerService->registerLayer(layerName);
	layerService->registerLayers(gameData->getEntityContainer());

	while (object) {
		if ( !checkNodeName(object, "object") ) {
			continue;
		}

		int gid = -1;

		SDL_Rect objBox;
		xml_attribute<> *attr = object->first_attribute();
		if ( checkAttrName(attr, "id") ) {
			attr = attr->next_attribute();
		}
		if ( checkAttrName(attr, "name") ) {
			attr = attr->next_attribute();
		}
		if ( checkAttrName(attr, "gid") ) {
			gid = atoi(attr->value());
			attr = attr->next_attribute();
		}

		objBox.x = static_cast<int>(atof(attr->value()));
		attr = attr->next_attribute();
		objBox.y = static_cast<int>(atof(attr->value()));
		attr = attr->next_attribute();
		objBox.w = static_cast<int>(atof(attr->value()));
		attr = attr->next_attribute();
		objBox.h = static_cast<int>(atof(attr->value()));

		Tileset *tileset = nullptr;
		for (auto it = map.tilesets.begin(); gid >= 0 && it != map.tilesets.end(); ++it) {
			if (gid < it->first) {
				break;
			}
			tileset = &(it->second);
		}

		MapTileObject* tileObj;
		if (tileset != nullptr) {
			std::cout << "Adding image object" << std::endl;
			tileObj = new MapTileObject(objBox.x, objBox.y, objBox.w, objBox.h, tileset->texture);

			int xoffset = (gid-1) * tileset->tileWidth;
			int xclip = xoffset % tileset->width;
			int yclip = tileset->tileHeight * ((xoffset - xclip) / tileset->width);

			SDL_Rect clip = { xclip, yclip, tileset->tileWidth, tileset->tileHeight };
			tileObj->setClip(clip);

		} else {
			tileObj = new MapTileObject(objBox.x, objBox.y, objBox.w, objBox.h, nullptr);
		}

		xml_node<> *properties = object->first_node();
		object = object->next_sibling();
		if (!properties) {
			continue;
		}

		xml_node<> *property = properties->first_node();
		while (property) {
			xml_attribute<> *nameAttr = property->first_attribute();
			xml_attribute<> *valueAttr = nameAttr->next_attribute();
			tileObj->setProperty(nameAttr->value(), checkAttrValue(valueAttr, "true") );
			if (checkAttrValue(nameAttr, "collidable")) {
				tileObj->getEntityProperties().setCollidable(checkAttrValue(valueAttr, "true"));
			}
			property = property->next_sibling();
		}
		gameData->getEntityContainer()->registerObject(tileObj, layerService->getLayerIndex(layerName));
	}
	node = node->next_sibling();

	return true;
}

std::string MapParser::getNameAttrValue(xml_node<> *node)
{
	for (xml_attribute<> *attribute = node->first_attribute(); attribute; attribute = attribute->next_attribute()) {
		if (checkAttrName(attribute, "name")) {
			return attribute->value();
		}
	}

	return "";
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
		tile.properties[nameAttr->value()] = checkAttrValue( valueAttr, "true");
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
		if ( !checkNodeName(object, "object") ) {
			continue;
		}

		// Skip first attr because it's id in a newer version of tiled
		xml_attribute<> *attr = object->first_attribute()->next_attribute();

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
		if ( checkAttrName(attr, "version") ) {
			map.version = attr->value();
		} else if ( checkAttrName(attr, "orientation") ) {
			map.orientation = attr->value();
		} else if ( checkAttrName(attr, "render0rder") ) {
			map.renderOrder = attr->value();
		} else if ( checkAttrName(attr, "width") ) {
			map.width = atoi(attr->value());
		} else if ( checkAttrName(attr, "height") ) {
			map.height = atoi(attr->value());
		} else if ( checkAttrName(attr, "tilewidth") ) {
			map.tileWidth = atoi(attr->value());
		} else if ( checkAttrName(attr, "tileheight") ) {
			map.tileHeight = atoi(attr->value());
		}
	}

	return true;
}

bool MapParser::checkNodeName(xml_node<> *node, std::string name) const
{
	return strcmp(node->name(), name.c_str()) == 0;
}

bool MapParser::checkAttrName(xml_attribute<> *attr, std::string name) const
{
	return strcmp(attr->name(), name.c_str()) == 0;
}

bool MapParser::checkAttrValue(xml_attribute<> *attr, std::string name) const
{
	return strcmp(attr->value(), name.c_str()) == 0;
}
