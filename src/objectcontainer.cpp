#include <cassert>
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
	removePtrFrom(object, objects);
}

void ObjectContainer::registerRenderable(Renderable* renderable)
{
	assert (!ptrExistsIn(renderable, renderables));
	renderables.push_back(renderable);
}

void ObjectContainer::unregisterRenderable(Renderable* renderable)
{
	removePtrFrom(renderable, renderables);
}

void ObjectContainer::registerEventHandler(EventHandler* handler)
{
	assert (!ptrExistsIn(handler, eventHandlers));
	eventHandlers.push_back(handler);
}

void ObjectContainer::unregisterEventHandler(EventHandler* handler)
{
	removePtrFrom(handler, eventHandlers);
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
