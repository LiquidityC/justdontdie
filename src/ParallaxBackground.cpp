#include <algorithm>
#include <string>
#include "ParallaxBackground.h"

void ParallaxBackground::updateSize()
{
	int width = 0,
		height = 0;

	for (size_t i = 0; i < textures.size(); i++) {
		flat2d::Texture *texture = textures[i];
		width = std::max(width, texture->getXpos() + texture->getWidth());
		height = std::max(height, texture->getYpos() + texture->getHeight());
	}

	entityProperties.setWidth(width);
	entityProperties.setHeight(height);
}

void ParallaxBackground::addTexture(int x, int y, std::string source, flat2d::RenderData *renderData)
{
	flat2d::Texture *texture = new flat2d::Texture(x, y);
	texture->loadFromFile(source, renderData->getRenderer());
	textures.push_back(texture);
	updateSize();
}

void ParallaxBackground::render(const flat2d::RenderData *renderData) const
{
	flat2d::Camera *camera = renderData->getCamera();

	for (size_t i = 0; i < textures.size(); i++) {
		flat2d::Texture *texture = textures[i];
		texture->setXpos(-1 * (camera->getXpos() + texture->getXpos()) / 1.5);
		texture->setYpos(-1 * (camera->getYpos() + texture->getYpos()) / 5);
		texture->render(renderData->getRenderer());
	}
}
