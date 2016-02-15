#include <string>

#include "MapTileObject.h"
#include "../npcs/Rocket.h"
#include "../CustomGameData.h"
#include "../LayerService.h"
#include "../Random.h"

void MapTileObject::setProperty(std::string prop, bool value)
{
	properties[prop] = value;
}

bool MapTileObject::hasProperty(std::string prop) const
{
	auto pair = properties.find(prop);
	if (pair == properties.end()) {
		return false;
	}
	return true;
}

void MapTileObject::init()
{
	if (hasProperty("rocketLauncher")) {
		rocketGenerator = new RocketGenerator();
	}
	if (hasProperty("destructible")) {
		tileBreakEmitter = new ParticleEmitter(ParticleType::FIRE_PARTICLE);
	}
}

void MapTileObject::preMove(const flat2d::GameData *gameData)
{
	if (rocketGenerator) {
		rocketGenerator->preMove(gameData, this);
	}

	if (hidden && hiddenTimer.isStarted() && hiddenTimer.getTicks() > 5000) {
		hiddenTimer.stop();
		hidden = false;
	}

	if (tileBreakEmitter) {
		tileBreakEmitter->emit(gameData, entityProperties.getBoundingBox());
	}
}

bool MapTileObject::isHidden() const
{
	return hidden;
}

void MapTileObject::hide()
{
	hidden = true;
	hiddenTimer.start();
}

void MapTileObject::destroy(const flat2d::GameData *data)
{
	setDead(true);
	tileBreakEmitter->setEmissionCount(200);
}

void MapTileObject::render(const flat2d::RenderData *data) const
{
	if (hidden) {
		return;
	}

	Entity::render(data);
#ifdef DEBUG
	if (texture == nullptr && !dead) {
		SDL_SetRenderDrawColor(data->getRenderer(), 0x00, 0x00, 0xFF, 0xFF );
		SDL_Rect boundingBox = entityProperties.getBoundingBox();
		boundingBox.x = data->getCamera()->getScreenXposFor(boundingBox.x);
		boundingBox.y = data->getCamera()->getScreenYposFor(boundingBox.y);
		SDL_RenderDrawRect( data->getRenderer(), &boundingBox);
	}
#endif
}
