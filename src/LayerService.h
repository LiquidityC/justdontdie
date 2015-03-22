#ifndef LAYERSERVICE_H_
#define LAYERSERVICE_H_

#include <flat/flat.h>

enum Layers {
	BACK,
	MID,
	FRONT,
	OVERLAY,
	LAST /* Not for use, just looping */
};

class LayerService
{
	public:
		void registerLayers(flat2d::EntityContainer *entityContainer);
};

#endif // LAYERSERVICE_H_
