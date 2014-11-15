#ifndef _OBJECT_CONTAINER_H
#define _OBJECT_CONTAINER_H

#include <iostream>
#include <map>
#include "GameObject.h"


namespace flat2d
{
	class RenderData;

	typedef int Layer;
	typedef std::map<std::string, GameObject*> ObjectList;

	class ObjectContainer 
	{
		private:
			ObjectList objects;
			ObjectList collidableObjects;
			std::map<Layer, ObjectList> layeredObjects;

		private:
			ObjectContainer(const ObjectContainer&); // Don't implement
			void operator=(const ObjectContainer&); // Don't implement

			void clearDeadObjects();

		public:
			static const int DEFAULT_LAYER = -1;

			ObjectContainer() { 
				ObjectList list;
				layeredObjects[-1] = list;
			}; 

			~ObjectContainer();

			void addLayer(unsigned int);

			void registerObject(GameObject*, Layer = DEFAULT_LAYER);
			void unregisterObject(GameObject*);

			void unregisterAllObjects();
			void unregisterAllObjectsFor(Layer);

			size_t getObjectCount();
			size_t getObjectCountFor(Layer);

			void preHandleObjects(const GameData*);
			void handleObjects(const SDL_Event&);
			void postHandleObjects(const GameData*);

			void preRenderObjects(const GameData*);
			void renderObjects(const RenderData*) const;
			void postRenderObjects(const GameData*);

			template <class Func>
				GameObject* checkAllCollidableObjects(Func func) const
				{
					for (auto it = collidableObjects.begin(); it != collidableObjects.end(); it++) {
						if (func(it->second)) {
							return it->second;
						}
					}
					return nullptr;
				}

			template <class Func>
				GameObject* checkAllObjects(Func func) const
				{
					for (auto it = objects.begin(); it != objects.end(); it++) {
						if (func(it->second)) {
							return it->second;
						}
					}
					return nullptr;
				}
	};
}

#endif
