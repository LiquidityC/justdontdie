#ifndef _OBJECT_CONTAINER_H
#define _OBJECT_CONTAINER_H

#include <vector>
#include <cassert>
#include "renderable.h"
#include "eventhandler.h"

class ObjectContainer : public Renderable, public EventHandler
{
	private:
		ObjectContainer() { }; 
		ObjectContainer(const ObjectContainer&); // Don't implement
		void operator=(const ObjectContainer&); // Don't implement

		std::vector<Object*> objects;
		std::vector<Renderable*> renderables;
		std::vector<EventHandler*> eventHandlers;

		void registerObject(Object*);
		void unregisterObject(Object*);

		template <class T>
			void removePtrFrom(T* ptr, std::vector<T*>& ptrs)
			{
				if (!ptrExistsIn(ptr, ptrs)) {
					return;
				}

				auto it = ptrs.begin();
				while (it != ptrs.end()) {
					if (*it == ptr) {
						ptrs.erase(it);
						break;
					}
					it++;
				}

				assert (!ptrExistsIn(ptr, ptrs));
			}

		template <class T>
			bool ptrExistsIn(T* ptr, std::vector<T*>& list)
			{
				auto it = list.begin();
				while (it != list.end()) {
					if (*it == ptr) {
						return true;
					}
					it++;
				}
				return false;
			}

	public:

		static ObjectContainer& getInstance()
		{
			static ObjectContainer instance;
			return instance;
		}

		~ObjectContainer();

		void registerRenderable(Renderable*);
		void unregisterRenderable(Renderable*);

		void registerEventHandler(EventHandler*);
		void unregisterEventHandler(EventHandler*);

		// Eventhandler override
		void preHandle();
		void handle(SDL_Event&);
		void postHandle();

		// Renderable override
		void preRender();
		void render(SDL_Surface&) const;
		void postRender();

		size_t getRenderableCount();
		size_t getEventHandlerCount();
};

#endif
