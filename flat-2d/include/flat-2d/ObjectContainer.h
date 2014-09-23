#ifndef _OBJECT_CONTAINER_H
#define _OBJECT_CONTAINER_H

#include <iostream>
#include <map>

#include "GameObject.h"

class ObjectContainer : public GameObject
{
	private:
		std::map<std::string, GameObject*> objects;

	private:
		ObjectContainer(const ObjectContainer&); // Don't implement
		void operator=(const ObjectContainer&); // Don't implement

	public:

		ObjectContainer() { }; 
		~ObjectContainer();

		void registerObject(GameObject*);
		void unregisterObject(GameObject*);

		size_t getObjectCount();

		// Eventhandler override
		void preHandle();
		void handle(const SDL_Event&);
		void postHandle();

		// Renderable override
		void preRender();
		void render(SDL_Renderer*) const;
		void postRender();

		void cleanNonVisibleObjects(int, int);

		template <class Func>
			bool checkAllObjects(Func func) const
			{
				for (auto it = objects.begin(); it != objects.end(); it++) {
					if (func(it->second)) {
						return true;
					}
				}
				return false;
			}
};

#endif
