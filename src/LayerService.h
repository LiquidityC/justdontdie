#ifndef LAYERSERVICE_H_
#define LAYERSERVICE_H_

#define FRONT_LAYER "frontLayer"
#define OVERLAY_LAYER "overlayLayer"

#include <flat/flat.h>
#include <map>
#include <string>

typedef std::map<std::string, unsigned int> LayerMap;

class LayerService
{
	private:
		LayerMap layerMap;

	public:
		void registerLayers(flat2d::EntityContainer *entityContainer);

		void registerLayer(std::string name);

		int getLayerIndex(std::string name);
};

#endif // LAYERSERVICE_H_
