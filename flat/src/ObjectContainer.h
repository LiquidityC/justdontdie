#ifndef _OBJECT_CONTAINER_H
#define _OBJECT_CONTAINER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include "LocationProperty.h"

namespace flat2d
{
	// Forward declarations
	class GameObject;
	class GameData;
	class RenderData;
	class RenderData;

	typedef int Layer;
	typedef std::map<std::string, GameObject*> ObjectList;
	typedef std::map<Layer, ObjectList> LayerMap;
	typedef std::map<LocationProperty, ObjectList> SpatialPartitionMap;

	class ObjectContainer 
	{
		private:
			unsigned int spatialPartitionDimension = 100;

			ObjectList objects;
			ObjectList collidableObjects;
			LayerMap layeredObjects;
			SpatialPartitionMap spatialPartitionMap;

			typedef std::function<bool (GameObject*)> GameObjectProcessor;

		private:
			ObjectContainer(const ObjectContainer&); // Don't implement
			void operator=(const ObjectContainer&); // Don't implement

			void clearDeadObjects();
			void registerCollidableObject(GameObject*);
			void registerObjectToSpatialPartitions(GameObject*);
			void addObjectToSpatialPartitionFor(GameObject*, int, int);
			void clearObjectFromCurrentPartitions(GameObject*);

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

			size_t getSpatialPartitionCount() const;

			void preHandleObjects(const GameData*);
			void handleObjects(const SDL_Event&);
			void postHandleObjects(const GameData*);

			void preRenderObjects(const GameData*);
			void renderObjects(const RenderData*) const;
			void postRenderObjects(const GameData*);

			void setSpatialPartitionDimension(unsigned int);

			GameObject* checkAllCollidableObjects(GameObjectProcessor) const;
			GameObject* checkAllObjects(GameObjectProcessor) const;
			GameObject* checkCollidablesFor(const GameObject*, GameObjectProcessor);
	};
}

#endif
