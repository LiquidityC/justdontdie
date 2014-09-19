#include "objectcontainer.h"

ObjectContainer::~ObjectContainer()
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		delete it->second;
	}
	objects.clear();
}

void ObjectContainer::registerObject(GameObject* object)
{
	if (objects.find(object->getStringId()) != objects.end()) {
		return;
	}
	objects[object->getStringId()] = object;
}

void ObjectContainer::unregisterObject(GameObject* object)
{
	objects.erase(object->getStringId());
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

void ObjectContainer::render(SDL_Surface& surface) const
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->render(surface);
	}
}

void ObjectContainer::postRender() 
{
	for (auto it = objects.begin(); it != objects.end(); it++) {
		it->second->postRender();
	}
}

size_t ObjectContainer::getObjectCount() {
	return objects.size();
}

void ObjectContainer::cleanNonVisibleObjects(int screenWidth, int screenHeight) 
{
	std::vector<std::string> itemsToErase;

	for (auto it = objects.begin(); it != objects.end(); it++) {
		SDL_Rect box = it->second->getBoundingBox();
		if (box.x + box.w > 0 && box.x < screenWidth) {
			continue;
		} else if (box.y + box.h > 0 && box.y < screenHeight) {
			continue;
		}
		delete it->second;
		it->second = NULL;

		itemsToErase.push_back(it->first);
	}

	for (auto it = itemsToErase.begin(); it != itemsToErase.end(); it++) {
		objects.erase(*it);
	}
}
