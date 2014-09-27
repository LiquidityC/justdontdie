#ifndef _OBJECT_CONTAINER_H
#define _OBJECT_CONTAINER_H

#include <iostream>
#include <map>

#include "GameObject.h"

namespace flat2d
{

	typedef int Layer;
	typedef std::map<std::string, GameObject*> ObjectList;

	class ObjectContainer : public GameObject
	{
		private:
			ObjectList objects;
			std::map<Layer, ObjectList> layeredObjects;

		private:
			ObjectContainer(const ObjectContainer&); // Don't implement
			void operator=(const ObjectContainer&); // Don't implement

		public:
			static const int DEFAULT_LAYER = -1;

			ObjectContainer() { 
				ObjectList list;
				layeredObjects[-1] = list;
			}; 

			~ObjectContainer();

			void addLayer(unsigned int);

			void registerObject(GameObject*, int = DEFAULT_LAYER);
			void unregisterObject(GameObject*);

			size_t getObjectCount(Layer = -2);

			// Eventhandler override
			void preHandle();
			void handle(const SDL_Event&);
			void postHandle();

			// Renderable override
			void preRender();
			void render(SDL_Renderer*) const;
			void postRender();

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
}

#endif
