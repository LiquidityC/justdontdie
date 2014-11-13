#ifndef _LAYER_SERVICE_H
#define _LAYER_SERVICE_H

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
		void registerLayers(flat2d::ObjectContainer *objectContainer);
};

#endif
