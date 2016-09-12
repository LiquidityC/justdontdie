#ifndef MAP_MAPGENERATOR_H_
#define	MAP_MAPGENERATOR_H_

#include <vector>
#include <string>

typedef std::vector<char> Room;
typedef std::vector<Room> Rooms;

class MapGenerator
{
	private:
		Rooms standardRooms;
		Rooms specialRooms;

		void parseRoomFiles();
		void parseRoomFile(const std::string& file);

	public:
		void loadMapData();

		void generateMap(const flat2d::GameData*, std::string tilemap) const;
};

#endif // MAP_MAPGENERATOR_H_
