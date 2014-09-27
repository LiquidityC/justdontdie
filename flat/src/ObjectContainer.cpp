#include <cassert>
#include "ObjectContainer.h"

using namespace flat2d;

ObjectContainer::~ObjectContainer()
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		delete it->second;
	}
	objects.clear();
	layeredObjects.clear();
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
	assert (layeredObjects.find(layer) != layeredObjects.end());

	objects[objId] = object;
	layeredObjects[layer][objId] = object;
}

void ObjectContainer::unregisterObject(GameObject* object)
{
	std::string objId = object->getStringId();
	objects.erase(objId);
	for (auto it = layeredObjects.begin(); it != layeredObjects.end(); it++) {
		it->second.erase(objId);
	}
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

size_t ObjectContainer::getObjectCount(Layer layer) 
{
	if (layer < DEFAULT_LAYER) {
		return objects.size();
	}

	if (layeredObjects.find(layer) == layeredObjects.end()) {
		return 0;
	}

	return layeredObjects[layer].size();
}
