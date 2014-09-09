#include <iostream>
#include "objectcontainer.h"

ObjectContainer::~ObjectContainer()
{
	while (!objects.empty()) {
		delete objects.back();
		objects.pop_back();
	}

	renderables.clear();
	eventHandlers.clear();
}

void ObjectContainer::registerObject(Object* object)
{
	if (ptrExistsIn(object, objects)) {
		return;
	}
	objects.push_back(object);
}

void ObjectContainer::unregisterObject(Object* object)
{
	if (!ptrExistsIn(object, renderables)
			&& !ptrExistsIn(object, eventHandlers)) 
	{
		removePtrFrom(object, objects);
	}
}

void ObjectContainer::registerRenderable(Renderable* renderable)
{
	if (ptrExistsIn(renderable, renderables)) {
		return;
	}
	registerObject(renderable);
	renderables.push_back(renderable);
}

void ObjectContainer::unregisterRenderable(Renderable* renderable)
{
	removePtrFrom(renderable, renderables);
	unregisterObject(renderable);
}

void ObjectContainer::registerEventHandler(EventHandler* handler)
{
	if (ptrExistsIn(handler, eventHandlers)) {
		return;
	}
	registerObject(handler);
	eventHandlers.push_back(handler);
}

void ObjectContainer::unregisterEventHandler(EventHandler* handler)
{
	removePtrFrom(handler, eventHandlers);
	unregisterObject(handler);
}

void ObjectContainer::preHandle()
{
	for (size_t i = 0; i < eventHandlers.size(); i++) {
		eventHandlers[i]->preHandle();
	}
}

void ObjectContainer::handle(SDL_Event& event)
{
	for (size_t i = 0; i < eventHandlers.size(); i++) {
		eventHandlers[i]->handle(event);
	}
}

void ObjectContainer::postHandle()
{
	for (size_t i = 0; i < eventHandlers.size(); i++) {
		eventHandlers[i]->postHandle();
	}
}

void ObjectContainer::preRender() 
{
	for (size_t i = 0; i < eventHandlers.size(); i++) {
		renderables[i]->preRender();
	}
}

void ObjectContainer::render(SDL_Surface& surface) const
{
	for (size_t i = 0; i < eventHandlers.size(); i++) {
		renderables[i]->render(surface);
	}
}

void ObjectContainer::postRender() 
{
	for (size_t i = 0; i < eventHandlers.size(); i++) {
		renderables[i]->postRender();
	}
}

size_t ObjectContainer::getObjectCount() {
	return objects.size();
}

size_t ObjectContainer::getRenderableCount() {
	return renderables.size();
}

size_t ObjectContainer::getEventHandlerCount() {
	return eventHandlers.size();
}
