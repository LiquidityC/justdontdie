#ifndef _RENDER_DATA_H
#define _RENDER_DATA_H

#include <SDL2/SDL.h>

namespace flat2d
{
	class Camera;
	class ObjectContainer;
	class CollisionDetector;

	class RenderData
	{
		private:
			SDL_Renderer *renderer;
			Camera *camera;
			ObjectContainer* objectContainer;
			CollisionDetector* collisionDetector;

		public:
			RenderData(SDL_Renderer *ren, Camera *cam, ObjectContainer *obc, CollisionDetector *cd) : 
				renderer(ren),
				camera(cam),
				objectContainer(obc), 
				collisionDetector(cd) { };

			SDL_Renderer* getRenderer() const { return renderer; };
			Camera* getCamera() const { return camera; };
			ObjectContainer* getObjectContainer() const { return objectContainer; };
			CollisionDetector* getCollisionDetector() const { return collisionDetector; };
	};
}

#endif
