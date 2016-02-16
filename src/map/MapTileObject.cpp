#include <string>

#include "MapTileObject.h"
#include "RocketGenerator.h"
#include "EnemyGenerator.h"
#include "../CustomGameData.h"
#include "../LayerService.h"
#include "../Random.h"

void MapTileObject::setProperty(std::string prop, std::string value)
{
	properties[prop] = value;
}

std::string MapTileObject::getProperty(std::string prop)
{
	return properties[prop];
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
		generators.push_back(new RocketGenerator());
	}
	if (hasProperty("enemyGenerator")) {
		generators.push_back(new EnemyGenerator());
	}
	if (hasProperty("destructible")) {
		tileBreakEmitter = new ParticleEmitter(ParticleType::FIRE_PARTICLE);
	}
}

void MapTileObject::preMove(const flat2d::GameData *gameData)
{
	for (auto generator : generators) {
		generator->preMove(gameData, this);
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

		// Draw spatial partitions
		SDL_SetRenderDrawColor(data->getRenderer(), 0xFF, 0x00, 0x00, 0xFF );
		const flat2d::EntityProperties::Areas currentAreas = entityProperties.getCurrentAreas();
		for(auto it = currentAreas.begin(); it != currentAreas.end(); it++) {
			SDL_Rect bounds = (*it).asSDLRect();
			bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
			bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
			SDL_RenderDrawRect( data->getRenderer(), &bounds );
		}
	}
#endif
}
