#ifndef _MAP_PARSER_H
#define _MAP_PARSER_H

#include <flat/flat.h>
#include <string>

#include "NewMacro.h"

class MapParser
{
	typedef struct {
		int id;
		bool collidable;
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
		bool createMapFrom(std::string, std::string, SDL_Renderer*);

	private:
		bool parseMapAttributes(rapidxml::xml_node<> *node);
		bool parseTileset(rapidxml::xml_node<> *node);
};

#endif
