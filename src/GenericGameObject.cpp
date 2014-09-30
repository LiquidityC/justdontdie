#include "GenericGameObject.h"

void GenericGameObject::init(SDL_Renderer *renderer, flat2d::Camera *camera)
{
	setCamera(camera);
}
