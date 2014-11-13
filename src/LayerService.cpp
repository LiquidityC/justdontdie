#include "LayerService.h"

void LayerService::registerLayers(flat2d::ObjectContainer *objectContainer)
{
	for (int i = Layers::BACK; i != Layers::LAST; i++) {
		objectContainer->addLayer(i);
	}
}
