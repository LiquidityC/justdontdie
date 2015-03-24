#ifndef ENTITYCONTAINER_H_
#define ENTITYCONTAINER_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include <string>
#include <functional>
#include "MapArea.h"

namespace flat2d
{
	// Forward declarations
	class Entity;
	class GameData;
	class RenderData;
	class DeltatimeMonitor;

	typedef int Layer;
	typedef std::map<std::string, Entity*> ObjectList;
	typedef std::map<Layer, ObjectList> LayerMap;
	typedef std::map<MapArea, ObjectList> SpatialPartitionMap;

	class EntityContainer
	{
		private:
			unsigned int spatialPartitionDimension = 100;

			DeltatimeMonitor *dtMonitor = nullptr;

			ObjectList objects;
			ObjectList collidableObjects;
			LayerMap layeredObjects;
			SpatialPartitionMap spatialPartitionMap;

			typedef std::function<bool (Entity*)> EntityProcessor;

		private:
			EntityContainer(const EntityContainer&); // Don't implement
			void operator=(const EntityContainer&); // Don't implement

			void clearDeadObjects();
			void registerCollidableObject(Entity *entity);
			void registerObjectToSpatialPartitions(Entity *entity);
			void addObjectToSpatialPartitionFor(Entity *entity, int x, int y);
			void clearObjectFromCurrentPartitions(Entity *entity);

		public:
			static const int DEFAULT_LAYER = -1;

			explicit EntityContainer(DeltatimeMonitor* dtm) : dtMonitor(dtm) {
				ObjectList list;
				layeredObjects[-1] = list;
			}

			~EntityContainer();

			void addLayer(unsigned int);

			void registerObject(Entity*, Layer = DEFAULT_LAYER);
			void unregisterObject(Entity *entity);

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

			Entity* checkAllCollidableObjects(EntityProcessor) const;
			Entity* checkAllObjects(EntityProcessor) const;
			Entity* checkCollidablesFor(const Entity*, EntityProcessor);
	};
} // namespace flat2d

#endif // ENTITYCONTAINER_H_
