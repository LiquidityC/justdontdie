#ifndef _MAP_PARSER_H
#define _MAP_PARSER_H

#include "NewMacro.h"
#include <flat/flat.h>
#include <string>

class ResourceContainer;

class MapParser
{
	typedef struct {
		int id;
		std::map<std::string, bool> properties;
		SDL_Rect collider;
	} Tile;

	typedef struct {
		std::string name;
		int firstgid;
		int tileWidth;
		int tileHeight;
		std::string sourcePath;
		int width;
		int height;
		std::map<int, Tile> tiles;
		SDL_Texture* texture;
	} Tileset;

	typedef struct {
		std::string version;
		std::string orientation;
		std::string renderOrder;
		int width;
		int height;
		int tileWidth;
		int tileHeight;
		std::map<int, Tileset> tilesets;
	} Map;

	private:
		Map map;

	public:
		bool createMapFrom(ResourceContainer*, std::string, std::string, flat2d::RenderData*);

	private:
		bool parseMapAttributes(rapidxml::xml_node<> *node);
		bool parseTileset(rapidxml::xml_node<> *node);
		void parseTileProperties(Tile&, rapidxml::xml_node<>*);
		void parseTileObjects(Tile&, rapidxml::xml_node<>*);
};

#endif
