#include <functional>
#include <cassert>
#include "GameObject.h"
#include "ObjectContainer.h"
#include "GameData.h"
#include "RenderData.h"
#include "LocationProperty.h"

using namespace flat2d;

ObjectContainer::~ObjectContainer()
{
	unregisterAllObjects();
}

void ObjectContainer::addLayer(unsigned int layer)
{
	Layer newLayer = static_cast<Layer>(layer);
	if (layeredObjects.find(newLayer) != layeredObjects.end()) {
		return;
	}
	ObjectList list;
	layeredObjects[newLayer] = list;
}

void ObjectContainer::registerObject(GameObject* object, Layer layer)
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
	if (object->isCollider()) {
		registerCollidableObject(object);
	}
}

void ObjectContainer::registerCollidableObject(GameObject* o)
{
	std::string objId = o->getStringId();
	collidableObjects[objId] = o;

	LocationProperty& locationProp = o->getLocationProperty();
	locationProp.getParents().clear();
	SDL_Rect b = locationProp.getBoundingBox();

	addObjectToSpatialPartitionFor(o, b.x, b.y);
	addObjectToSpatialPartitionFor(o, b.x + b.w, b.y);
	addObjectToSpatialPartitionFor(o, b.x, b.y + b.h);
	addObjectToSpatialPartitionFor(o, b.x + b.w, b.y + b.h);

	locationProp.setOnLocationChange(
			[]() {
			std::cout << "Re-registering object" << std::endl;
			//registerCollidableObject(o);
			});

}

void ObjectContainer::addObjectToSpatialPartitionFor(GameObject* o, int x, int y)
{
	std::string objId = o->getStringId();

	unsigned int xcord = (x - (x % spatialPartitionDimension));
	unsigned int ycord = (y - (y % spatialPartitionDimension));

	// Find and make sure partition exists
	LocationProperty loc(xcord, ycord, spatialPartitionDimension);
	if (spatialPartitionMap.find(loc) == spatialPartitionMap.end()) {
		spatialPartitionMap[loc] = ObjectList();
	}

	// The object is already in this partition
	if (spatialPartitionMap[loc].find(objId) != spatialPartitionMap[loc].end()) {
		return;
	}

	LocationProperty& objLocationProperty = o->getLocationProperty();
	objLocationProperty.getParents().push_back(loc);

	spatialPartitionMap[loc][objId] = o;
}

void ObjectContainer::setSpatialPartitionDimension(unsigned int i)
{
	spatialPartitionDimension = i;
}

void ObjectContainer::unregisterObject(GameObject* object)
{
	std::string objId = object->getStringId();
	objects.erase(objId);
	if (object->isCollider()) {
		collidableObjects.erase(objId);
	}

	LocationProperty::Parents locationProps = object->getLocationProperty().getParents();
	for (auto it = locationProps.begin(); it != locationProps.end(); it++) {
		spatialPartitionMap[*it].erase(objId);
	}

	for (auto it = layeredObjects.begin(); it != layeredObjects.end(); it++) {
		it->second.erase(objId);
	}
}

void ObjectContainer::unregisterAllObjects()
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

void ObjectContainer::unregisterAllObjectsFor(Layer layer)
{
	if (layeredObjects.find(layer) == layeredObjects.end()) {
		return;
	}

	for (auto it = layeredObjects[layer].begin(); it != layeredObjects[layer].end(); it++) {
		std::string objId = it->second->getStringId();
		objects.erase(objId);
		if (it->second->isCollider()) {
			collidableObjects.erase(objId);
		}
		delete it->second;
	}

	layeredObjects[layer].clear();
}

void ObjectContainer::preHandleObjects(const GameData *gameData)
{
	clearDeadObjects();
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->preHandle(gameData);
	}
}

void ObjectContainer::handleObjects(const SDL_Event& event)
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->handle(event);
	}
}

void ObjectContainer::postHandleObjects(const GameData *gameData)
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->postHandle(gameData);
	}
}

void ObjectContainer::preRenderObjects(const GameData* data) 
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->preRender(data);
	}
}

void ObjectContainer::renderObjects(const RenderData* data) const
{
	for (auto it1 = layeredObjects.begin(); it1 != layeredObjects.end(); it1++) {
		for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			it2->second->render(data);
		}
	}
}

void ObjectContainer::postRenderObjects(const GameData* data) 
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->postRender(data);
	}
}

size_t ObjectContainer::getObjectCount() 
{
	return objects.size();
}

size_t ObjectContainer::getObjectCountFor(Layer layer) 
{
	if (layeredObjects.find(layer) == layeredObjects.end()) {
		return 0;
	}

	return layeredObjects[layer].size();
}

void ObjectContainer::clearDeadObjects()
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
		objectsToErase.push_back(objId);
		delete it->second;
	}

	for (auto it = objectsToErase.begin(); it != objectsToErase.end(); it++) {
		objects.erase(*it);
	}
}

size_t ObjectContainer::getSpatialPartitionCount() const
{
	return spatialPartitionMap.size();
}
