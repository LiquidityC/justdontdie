#ifndef _OBJECT_CONTAINER_H
#define _OBJECT_CONTAINER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <map>

/**
 * Forward declarations
 */
namespace flat2d
{
	class GameObject;
	class GameData;
	class RenderData;
	class RenderData;
	class Coordinate;

	typedef int Layer;
	typedef std::map<std::string, GameObject*> ObjectList;
	typedef std::map<Layer, ObjectList> LayerMap;
}

bool operator<(const flat2d::Coordinate&, const flat2d::Coordinate&);

/**
 * Implementation
 */
namespace flat2d
{
	class Coordinate
	{
		private:
			int x, y;

		public:
			Coordinate(int xp, int yp) : x(xp), y(yp) { };
			friend bool (::operator<)(const flat2d::Coordinate& a, const flat2d::Coordinate& b);
	};

	class ObjectContainer 
	{
		private:
			ObjectList objects;
			ObjectList collidableObjects;
			LayerMap layeredObjects;

		private:
			ObjectContainer(const ObjectContainer&); // Don't implement
			void operator=(const ObjectContainer&); // Don't implement

			void clearDeadObjects();

		public:
			static const int DEFAULT_LAYER = -1;

			ObjectContainer() { 
				ObjectList list;
				layeredObjects[-1] = list;
			}; 

			~ObjectContainer();

			void addLayer(unsigned int);

			void registerObject(GameObject*, Layer = DEFAULT_LAYER);
			void unregisterObject(GameObject*);

			void unregisterAllObjects();
			void unregisterAllObjectsFor(Layer);

			size_t getObjectCount();
			size_t getObjectCountFor(Layer);

			void preHandleObjects(const GameData*);
			void handleObjects(const SDL_Event&);
			void postHandleObjects(const GameData*);

			void preRenderObjects(const GameData*);
			void renderObjects(const RenderData*) const;
			void postRenderObjects(const GameData*);

			template <class Func>
				GameObject* checkAllCollidableObjects(Func func) const
				{
					for (auto it = collidableObjects.begin(); it != collidableObjects.end(); it++) {
						if (func(it->second)) {
							return it->second;
						}
					}
					return nullptr;
				}

			template <class Func>
				GameObject* checkAllObjects(Func func) const
				{
					for (auto it = objects.begin(); it != objects.end(); it++) {
						if (func(it->second)) {
							return it->second;
						}
					}
					return nullptr;
				}
	};
}

#endif
