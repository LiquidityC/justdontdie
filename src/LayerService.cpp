#include "LayerService.h"
#include <string>

void LayerService::registerLayers(flat2d::EntityContainer *entityContainer)
{
	for (auto it = layerMap.begin(); it != layerMap.end(); ++it) {
		entityContainer->addLayer(it->second);
	}
}

void LayerService::registerLayer(std::string name)
{
	static unsigned int index = 0;
	layerMap[name] = index;
	index++;
}

int LayerService::getLayerIndex(std::string name)
{
	if ( layerMap.find(name) == layerMap.end() ) {
		return -1;
	}
	return layerMap[name];
}
