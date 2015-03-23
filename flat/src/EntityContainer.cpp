#include <functional>
#include <cassert>
#include <string>
#include <vector>

#include "Entity.h"
#include "EntityContainer.h"
#include "GameData.h"
#include "RenderData.h"
#include "EntityProperties.h"


namespace flat2d
{
	EntityContainer::~EntityContainer()
	{
		unregisterAllObjects();
	}

	void EntityContainer::addLayer(unsigned int layer)
	{
		Layer newLayer = static_cast<Layer>(layer);
		if (layeredObjects.find(newLayer) != layeredObjects.end()) {
			return;
		}
		ObjectList list;
		layeredObjects[newLayer] = list;
	}

	void EntityContainer::registerObject(Entity* object, Layer layer)
	{
		std::string objId = object->getStringId();
		if (objects.find(objId) != objects.end()) {
			return;
		}

		// Make sure this layer exists
		if (layeredObjects.find(layer) == layeredObjects.end()) {
			return;
		}

		objects[objId] = object;
		layeredObjects[layer][objId] = object;
		registerObjectToSpatialPartitions(object);
		if (object->getEntityProperties().isCollidable()) {
			registerCollidableObject(object);
		}
	}

	void EntityContainer::registerCollidableObject(Entity* o)
	{
		collidableObjects[o->getStringId()] = o;
	}

	void EntityContainer::registerObjectToSpatialPartitions(Entity *o)
	{
		EntityProperties& locationProp = o->getEntityProperties();
		SDL_Rect b = locationProp.getBoundingBox();

		addObjectToSpatialPartitionFor(o, b.x, b.y);
		addObjectToSpatialPartitionFor(o, b.x + b.w, b.y);
		addObjectToSpatialPartitionFor(o, b.x, b.y + b.h);
		addObjectToSpatialPartitionFor(o, b.x + b.w, b.y + b.h);

		locationProp.setOnLocationChange(
				[this, o]() {
				clearObjectFromCurrentPartitions(o);
				registerObjectToSpatialPartitions(o);
				});
	}

	void EntityContainer::clearObjectFromCurrentPartitions(Entity *o)
	{
		EntityProperties::Areas& currentAreas = o->getEntityProperties().getCurrentAreas();
		for (auto it = currentAreas.begin(); it != currentAreas.end(); it++) {
			spatialPartitionMap[*it].erase(o->getStringId());
		}
		currentAreas.clear();
	}

	void EntityContainer::addObjectToSpatialPartitionFor(Entity* o, int x, int y)
	{
		std::string objId = o->getStringId();

		unsigned int xcord = (x - (x % spatialPartitionDimension));
		unsigned int ycord = (y - (y % spatialPartitionDimension));

		// Find and make sure partition exists
		MapArea area(xcord, ycord, spatialPartitionDimension);
		if (spatialPartitionMap.find(area) == spatialPartitionMap.end()) {
			spatialPartitionMap[area] = ObjectList();
		}

		// The object is already in this partition
		if (spatialPartitionMap[area].find(objId) != spatialPartitionMap[area].end()) {
			return;
		}

		EntityProperties& objEntityProperties = o->getEntityProperties();
		objEntityProperties.getCurrentAreas().push_back(area);

		spatialPartitionMap[area][objId] = o;
	}

	void EntityContainer::setSpatialPartitionDimension(unsigned int i)
	{
		spatialPartitionDimension = i;
	}

	void EntityContainer::unregisterObject(Entity* object)
	{
		std::string objId = object->getStringId();
		objects.erase(objId);
		if (object->getEntityProperties().isCollidable()) {
			collidableObjects.erase(objId);
		}

		clearObjectFromCurrentPartitions(object);

		for (auto it = layeredObjects.begin(); it != layeredObjects.end(); it++) {
			it->second.erase(objId);
		}
	}

	void EntityContainer::unregisterAllObjects()
	{
		for(auto it = objects.begin(); it != objects.end(); it++) {
			delete it->second;
		}
		objects.clear();
		collidableObjects.clear();
		spatialPartitionMap.clear();
		for (auto it = layeredObjects.begin(); it != layeredObjects.end(); it++) {
			it->second.clear();
		}
	}

	void EntityContainer::unregisterAllObjectsFor(Layer layer)
	{
		if (layeredObjects.find(layer) == layeredObjects.end()) {
			return;
		}

		for (auto it = layeredObjects[layer].begin(); it != layeredObjects[layer].end(); it++) {
			std::string objId = it->second->getStringId();
			objects.erase(objId);
			if (it->second->getEntityProperties().isCollidable()) {
				collidableObjects.erase(objId);
			}
			delete it->second;
		}

		layeredObjects[layer].clear();
	}

	void EntityContainer::preHandleObjects(const GameData *gameData)
	{
		clearDeadObjects();
		for (auto it = objects.begin(); it != objects.end(); it++) {
			it->second->preHandle(gameData);
		}
	}

	void EntityContainer::handleObjects(const SDL_Event& event)
	{
		for (auto it = objects.begin(); it != objects.end(); it++) {
			it->second->handle(event);
		}
	}

	void EntityContainer::postHandleObjects(const GameData *gameData)
	{
		for (auto it = objects.begin(); it != objects.end(); it++) {
			it->second->postHandle(gameData);
		}
	}

	void EntityContainer::preRenderObjects(const GameData* data)
	{
		for (auto it = objects.begin(); it != objects.end(); it++) {
			it->second->preRender(data);
		}
	}

	void EntityContainer::renderObjects(const RenderData* data) const
	{
		for (auto it1 = layeredObjects.begin(); it1 != layeredObjects.end(); it1++) {
			for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
				it2->second->render(data);
			}
		}
	}

	void EntityContainer::postRenderObjects(const GameData* data)
	{
		for (auto it = objects.begin(); it != objects.end(); it++) {
			it->second->postRender(data);
		}
	}

	size_t EntityContainer::getObjectCount()
	{
		return objects.size();
	}

	size_t EntityContainer::getObjectCountFor(Layer layer)
	{
		if (layeredObjects.find(layer) == layeredObjects.end()) {
			return 0;
		}

		return layeredObjects[layer].size();
	}

	void EntityContainer::clearDeadObjects()
	{
		std::vector<std::string> objectsToErase;
		for (auto it = objects.begin(); it != objects.end(); it++) {
			if (!it->second->isDead()) {
				continue;
			}
			std::string objId = it->first;
			collidableObjects.erase(objId);
			for (auto layerIt = layeredObjects.begin(); layerIt != layeredObjects.end(); layerIt++) {
				layerIt->second.erase(objId);
			}
			clearObjectFromCurrentPartitions(it->second);
			objectsToErase.push_back(objId);
			delete it->second;
		}

		for (auto it = objectsToErase.begin(); it != objectsToErase.end(); it++) {
			objects.erase(*it);
		}
	}

	size_t EntityContainer::getSpatialPartitionCount() const
	{
		return spatialPartitionMap.size();
	}

	Entity* EntityContainer::checkAllObjects(EntityProcessor func) const
	{
		for (auto it = objects.begin(); it != objects.end(); it++) {
			if (func(it->second)) {
				return it->second;
			}
		}
		return nullptr;
	}

	Entity* EntityContainer::checkAllCollidableObjects(EntityProcessor func) const
	{
		for (auto it = collidableObjects.begin(); it != collidableObjects.end(); it++) {
			if (func(it->second)) {
				return it->second;
			}
		}
		return nullptr;
	}

	Entity* EntityContainer::checkCollidablesFor(const Entity* source, EntityProcessor func)
	{
		const EntityProperties::Areas& currentAreas = source->getEntityProperties().getCurrentAreas();
		for (auto areaIter = currentAreas.begin(); areaIter != currentAreas.end(); areaIter++) {
			for (auto objectIter = spatialPartitionMap[*areaIter].begin();
					objectIter != spatialPartitionMap[*areaIter].end();
					objectIter++)
			{
				if (objectIter->second->getEntityProperties().isCollidable() && func(objectIter->second)) {
					return objectIter->second;
				}
			}
		}
		return nullptr;
	}
} // namespace flat2d
