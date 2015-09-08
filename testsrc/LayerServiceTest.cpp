#include <flat/flat.h>
#include "catch.hpp"
#include "../src/LayerService.h"

TEST_CASE ( "Test layer service", "[layers]" )
{
	LayerService *layerService = new LayerService();

	layerService->registerLayer("1");
	layerService->registerLayer("2");
	layerService->registerLayer("3");
	layerService->registerLayer("4");

	SECTION ( "Test layer indexing", "[index][layers]" )
	{
		REQUIRE ( 0 == layerService->getLayerIndex("1") );
		REQUIRE ( 1 == layerService->getLayerIndex("2") );
		REQUIRE ( 2 == layerService->getLayerIndex("3") );
		REQUIRE ( 3 == layerService->getLayerIndex("4") );
		REQUIRE ( -1 == layerService->getLayerIndex("5") );
	}

	delete layerService;
}
