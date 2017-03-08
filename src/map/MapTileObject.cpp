#include <string>

#include "MapTileObject.h"
#include "RocketGenerator.h"
#include "OneShotEnemyGenerator.h"
#include "ProximityEnemyGenerator.h"
#include "../CustomGameData.h"
#include "../LayerService.h"
#include "../Random.h"

void MapTileObject::setProperty(std::string prop, std::string value)
{
	properties[prop] = value;
}

std::string MapTileObject::getProperty(std::string prop) const
{
	return properties.find(prop)->second;
}

bool MapTileObject::hasProperty(std::string prop) const
{
	if (properties.find(prop) == properties.end()) {
		return false;
	}
	return true;
}

void MapTileObject::initTile()
{
	if (hasProperty("rocketLauncher")) {
		generators.push_back(new RocketGenerator());
	}
	if (hasProperty("enemyGenerator")) {
		std::string type = getProperty("enemyGenerator");
		if (type == "oneShot") {
			generators.push_back(new OneShotEnemyGenerator());
		} else if (type == "proximity") {
			generators.push_back(new ProximityEnemyGenerator());
		}
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
}
