#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "../Macros.h"
#include "MapParser.h"
#include "MapTileObject.h"
#include "../CustomGameData.h"
#include "../ResourceContainer.h"
#include "../LayerService.h"

using rapidxml::file;
using rapidxml::xml_document;
using rapidxml::xml_node;
using rapidxml::xml_attribute;

bool MapParser::createMapFrom(flat2d::GameData *gameData, std::string dir, std::string filename)
{
	this->dir = dir;
	this->filename = filename;
	flat2d::RenderData *renderData = gameData->getRenderData();

	file<> xmlFile((dir + filename).c_str());
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	xml_node<> *node = doc.first_node();
	if ( !checkNodeName(node, "map") ) {
		std::cerr << "Root node not named 'map' parse failed: '" << node->name() << "'" << std::endl;
		return false;
	}

	// Get map data
	if (!parseMapAttributes(node)) {
		return false;
	}

	// Get tileset data
	parseTilesets(node);

	renderData->getCamera()->setMapDimensions(map.width * map.tileWidth, map.height * map.tileHeight);

	// Parse image layers
	if (!parseImageLayers(node, gameData)) {
		std::cerr << "Failed to parse image layers" << std::endl;
		return false;
	}

	// Parse all the layers
	for (xml_node<> *layer = node->first_node("layer"); layer; layer = layer->next_sibling("layer")) {
		if (!parseLayer(layer, gameData)) {
			std::cerr << "Failed to parse layers" << std::endl;
			return false;
		}
	}

	// Parse all object groups
	for (xml_node<> *objectgroup = node->first_node("objectgroup");
			objectgroup;
			objectgroup = objectgroup->next_sibling("objectgroup"))
	{
		if (!parseObjectLayer(objectgroup, gameData)) {
			std::cerr << "Failed to parse objectgroup" << std::endl;
			return false;
		}
	}

	mergeHorizontalColliders(gameData);

	return true;
}

MapParser::Matrix MapParser::populateCollidableMatrix(const flat2d::GameData *gameData, int layer)
{
		Matrix matrix;
		for (int i = 0; i < map.width; i++) {
			std::vector<MapTileObject*> column(map.height, nullptr);
			matrix.push_back(column);
		}

		gameData->getEntityContainer()->iterateCollidablesIn(layer,
				[&matrix, this](flat2d::Entity *entity) {
					if (entity->getType() != EntityType::TILE) {
						return;
					}

					flat2d::EntityProperties& props = entity->getEntityProperties();
					flat2d::EntityShape colShape = props.getColliderShape();
					if (colShape.w != map.tileWidth || colShape.h != map.tileHeight) {
						return;
					}

					int x = props.getXpos();
					int y = props.getYpos();
					if (x % map.tileWidth != 0 || y % map.tileHeight != 0) {
						return;
					}
					x = x / map.tileWidth;
					y = y / map.tileHeight;

					matrix[x][y] = static_cast<MapTileObject*>(entity);
				});

		return matrix;
}

void MapParser::mergeHorizontalColliders(const flat2d::GameData *gameData)
{
	flat2d::EntityContainer *container = gameData->getEntityContainer();

	for (auto& layer : container->getLayerKeys()) {
		Matrix matrix = populateCollidableMatrix(gameData, layer);

		for (int j = 0; j < map.height; j++) {
			int first = -1;
			for (size_t i = 0; i < matrix.size(); i++) {
				if (first < 0 && matrix[i][j] != nullptr) {
					first = i;
					continue;
				}
				if ((first >= 0 && matrix[i][j] == nullptr)
						|| (first >= 0 && static_cast<unsigned int>(j) == matrix[i].size() - 1))
				{
					for (unsigned int k = static_cast<unsigned int>(first); k < i; k++) {
						matrix[k][j]->getEntityProperties().setCollidable(false);
					}

					int width = map.tileWidth * (i - first);
					MapTileObject *collider = new MapTileObject(first * map.tileWidth, j * map.tileHeight,
							width, map.tileHeight, nullptr);
					flat2d::EntityShape colliderShape = { 0, 0, width, map.tileHeight };
					collider->getEntityProperties().setColliderShape(colliderShape);
					collider->getEntityProperties().setCollidable(true);
					container->registerObject(collider, layer);

					first = -1;
				}
			}
		}
	}
	container->repopulateCollidables();
}

/*
 * TODO(Linus): Implement this if it needs to be used.
 */
bool MapParser::parseImageLayers(xml_node<> *node, flat2d::GameData *gameData)
{
	xml_node<> *imageLayer = node->first_node("imagelayer");
	if (!imageLayer) {
		return true;
	}

	std::cerr << __FILE__ << "::parseImageLayers() not properly implemented" << std::endl;

	CustomGameData *customGameData = static_cast<CustomGameData*>(gameData->getCustomGameData());
	LayerService *layerService = customGameData->getLayerService();

	std::string layerName = getNameAttrValue(imageLayer);
	layerService->registerLayer(layerName);
	layerService->registerLayers(gameData->getEntityContainer());

	do {
		//xml_node<> *image = imageLayer->first_node("image");
		//xml_attribute<> *source = image->first_attribute("source");
		//xml_attribute<> *offsetx = imageLayer->first_attribute("offsetx");
		//xml_attribute<> *offsety = imageLayer->first_attribute("offsety");

		//int x = atoi(offsetx->value());
		//int y = atoi(offsety->value());
	} while ((imageLayer = imageLayer->next_sibling("imagelayer")) != nullptr);

	//int layerIndex = layerService->getLayerIndex(layerName);

	return true;
}

bool MapParser::parseTilesets(xml_node<> *node)
{
	for (xml_node<> *tilesetNode = node->first_node("tileset");
			tilesetNode;
			tilesetNode = tilesetNode->next_sibling("tileset"))
	{
		if (!parseTileset(tilesetNode)) {
			return false;
		}
	}

	return true;
}

bool MapParser::parseTileset(xml_node<> *node)
{
	Tileset tileset;
	tileset.texture = nullptr;
	xml_node<> *tilesetNode = node;

	// Get first gid
	tileset.firstgid = atoi(tilesetNode->first_attribute("firstgid")->value());

	// Check if it's an external source
	xml_document<> doc;
	file<> *xmlFile = nullptr;
	if ( tilesetNode->first_attribute("source")) {
		xml_attribute<> *source = tilesetNode->first_attribute("source");
		std::string completePath = dir + source->value();
		xmlFile = new file<>(completePath.c_str());
		doc.parse<0>(xmlFile->data());
		tilesetNode = doc.first_node();
	}

	tileset.name = tilesetNode->first_attribute("name")->value();
	tileset.tileWidth = atoi(tilesetNode->first_attribute("tilewidth")->value());
	tileset.tileHeight = atoi(tilesetNode->first_attribute("tileheight")->value());
	tileset.tileCount = atoi(tilesetNode->first_attribute("tilecount")->value());

	xml_node<> *imageNode = tilesetNode->first_node("image");
	if (!imageNode) {
		std::cerr << "Node not named 'image' parse failed: '" << imageNode->name() << "'" << std::endl;
		return false;
	}

	tileset.sourcePath = dir + imageNode->first_attribute("source")->value();
	tileset.width = atoi(imageNode->first_attribute("width")->value());
	tileset.height = atoi(imageNode->first_attribute("height")->value());

	// Parse tiles if exists
	int gid = tileset.firstgid;
	for (xml_node<> *tileNode = tilesetNode->first_node("tile"); tileNode; tileNode = tileNode->next_sibling("tile")) {
		int tileId = atoi(tileNode->first_attribute("id")->value());

		// Add leading empty tiles
		while (gid <= tileId) {
			Tile emptyTile;
			emptyTile.id = gid - 1;
			tileset.tiles[gid] = emptyTile;
			gid++;
		}

		Tile tile;
		tile.id = tileId;
		parseTileProperties(tile, tileNode->first_node("properties"));
		parseTileObjects(tile, tileNode->first_node("objectgroup"));

		tileset.tiles[gid] = tile;
		gid++;
	}

	map.tilesets[tileset.firstgid] = tileset;

	if (xmlFile) {
		delete xmlFile;
	}

	return true;
}

bool MapParser::parseLayer(xml_node<> *node, flat2d::GameData *gameData)
{
	CustomGameData *customGameData = static_cast<CustomGameData*>(gameData->getCustomGameData());
	LayerService *layerService = customGameData->getLayerService();
	ResourceContainer *resourceContainer = customGameData->getResourceContainer();
	flat2d::RenderData *renderData = gameData->getRenderData();

	// Extract layer name
	std::string layerName = getNameAttrValue(node);
	layerService->registerLayer(layerName);
	layerService->registerLayers(gameData->getEntityContainer());


	std::map<std::string, std::string> layerProps = getLayerProperties(node);
	int depth = 0;
	if (layerProps.find("depth") != layerProps.end()) {
		depth = atoi(layerProps["depth"].c_str());
	}

	int row = 0;
	int col = 0;
	xml_node<> *data = node->first_node("data");
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
					tileset->sourcePath, renderData->getRenderer());
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

		tileObj->getEntityProperties().setDepth(depth);

		// Set properties
		for (auto property : tile->properties) {
			tileObj->setProperty(property.first, property.second);
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
		tileObj->initTile();
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
	CustomGameData *customGameData = static_cast<CustomGameData*>(gameData->getCustomGameData());
	LayerService *layerService = customGameData->getLayerService();
	ResourceContainer *resourceContainer = customGameData->getResourceContainer();

	// Get the layer name
	std::string layerName = getNameAttrValue(node);
	layerService->registerLayer(layerName);
	layerService->registerLayers(gameData->getEntityContainer());

	xml_node<> *object = node->first_node("object");
	while (object) {
		if ( !checkNodeName(object, "object") ) {
			continue;
		}

		int gid = -1;

		SDL_Rect objBox;
		if ( object->first_attribute("gid") ) {
			gid = atoi(object->first_attribute("gid")->value());
		}

		objBox.x = static_cast<int>(atof(object->first_attribute("x")->value()));
		objBox.y = static_cast<int>(atof(object->first_attribute("y")->value()));
		objBox.w = static_cast<int>(atof(object->first_attribute("width")->value()));
		objBox.h = static_cast<int>(atof(object->first_attribute("height")->value()));

		Tileset *tileset = nullptr;
		for (auto it = map.tilesets.begin(); gid >= 0 && it != map.tilesets.end(); ++it) {
			if (gid < it->first) {
				break;
			}
			tileset = &(it->second);
		}

		MapTileObject* tileObj;
		if (tileset != nullptr) {
			if (tileset->texture == nullptr) {
				SDL_Texture* texture = flat2d::MediaUtil::loadTexture(
						tileset->sourcePath, gameData->getRenderData()->getRenderer());
				tileset->texture = texture;
				resourceContainer->addTexture(texture);
			}

			tileObj = new MapTileObject(objBox.x, objBox.y, objBox.w, objBox.h, tileset->texture);

			int xoffset = (tileset->firstgid - gid) * tileset->tileWidth;
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
			tileObj->setProperty(nameAttr->value(), valueAttr->value());
			if (checkAttrValue(nameAttr, "collidable")) {
				tileObj->getEntityProperties().setCollidable(checkAttrValue(valueAttr, "true"));
			}
			property = property->next_sibling();
		}
		tileObj->initTile();
		gameData->getEntityContainer()->registerObject(tileObj, layerService->getLayerIndex(layerName));
	}
	node = node->next_sibling();

	return true;
}

std::map<std::string, std::string> MapParser::getLayerProperties(xml_node<> *layer)
{
	std::map<std::string, std::string> props;
	xml_node<> *properties = layer->first_node("properties");
	if (!properties) {
		return props;
	}

	for (xml_node<> *prop = properties->first_node(); prop; prop = prop->next_sibling()) {
		xml_attribute<> *name = prop->first_attribute("name");
		xml_attribute<> *value = prop->first_attribute("value");
		props[name->value()] = value->value();
	}

	return props;
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
