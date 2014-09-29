#include <cassert>
#include "ObjectContainer.h"

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
		collidableObjects[objId] = object;
	}
}

void ObjectContainer::unregisterObject(GameObject* object)
{
	std::string objId = object->getStringId();
	objects.erase(objId);
	if (object->isCollider()) {
		collidableObjects.erase(objId);
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

void ObjectContainer::preHandle()
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->preHandle();
	}
}

void ObjectContainer::handle(const SDL_Event& event)
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->handle(event);
	}
}

void ObjectContainer::postHandle()
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->postHandle();
	}
}

void ObjectContainer::preRender() 
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->preRender();
	}
}

void ObjectContainer::render(SDL_Renderer* renderer) const
{
	for (auto it1 = layeredObjects.begin(); it1 != layeredObjects.end(); it1++) {
		for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			it2->second->render(renderer);
		}
	}
}

void ObjectContainer::postRender() 
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->postRender();
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
