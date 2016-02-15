#ifndef MAP_MAPPARSER_H_
#define MAP_MAPPARSER_H_

#include <flat/flat.h>
#include <string>
#include <map>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

class ResourceContainer;

class MapParser
{
	typedef struct {
		int id;
		std::map<std::string, std::string> properties;
		flat2d::EntityShape collider = { 0, 0, 0, 0 };
	} Tile;

	typedef struct {
		std::string name;
		int firstgid;
		int tileWidth;
		int tileHeight;
		int tileCount;
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
		std::string dir;
		std::string filename;

	public:
		bool createMapFrom(flat2d::GameData*, std::string, std::string);

	private:
		bool parseMapAttributes(rapidxml::xml_node<> *node);

		bool parseTilesets(rapidxml::xml_node<> *node);
		bool parseTileset(rapidxml::xml_node<> *node);

		std::string getNameAttrValue(rapidxml::xml_node<> *node);
		bool parseImageLayers(rapidxml::xml_node<> *node, flat2d::GameData*);
		bool parseLayer(rapidxml::xml_node<> *node, flat2d::GameData*);
		bool parseObjectLayer(rapidxml::xml_node<> *node, flat2d::GameData*);

		void parseTileProperties(Tile&, rapidxml::xml_node<>*);
		void parseTileObjects(Tile&, rapidxml::xml_node<>*);

		std::map<std::string, std::string> getLayerProperties(rapidxml::xml_node<>*);

		bool checkNodeName( rapidxml::xml_node<>*, std::string) const;
		bool checkAttrName( rapidxml::xml_attribute<>*, std::string) const;
		bool checkAttrValue( rapidxml::xml_attribute<>*, std::string) const;
};

#endif // MAP_MAPPARSER_H_
