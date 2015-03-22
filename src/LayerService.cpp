#include "LayerService.h"

void LayerService::registerLayers(flat2d::EntityContainer *entityContainer)
{
	for (int i = Layers::BACK; i != Layers::LAST; i++) {
		entityContainer->addLayer(i);
	}
}
