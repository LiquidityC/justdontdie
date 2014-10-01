#include "RenderData.h"

using namespace flat2d;

SDL_Renderer* RenderData::getRenderer() const
{
	return renderer;
}

Camera* RenderData::getCamera() const
{
	 return camera;
}
