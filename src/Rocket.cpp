#include "Rocket.h"
#include "EntityType.h"
#include "MapTileObject.h"
#include "Soldier.h"
#include "CustomGameData.h"
#include "SoundMappings.h"

void Rocket::init(const flat2d::GameData *data)
{
	setTexture(flat2d::MediaUtil::loadTexture("resources/textures/rocket.png",
				data->getRenderData()->getRenderer()));
	deathTimer.start();
}

void Rocket::preMove(const flat2d::GameData *data)
{
	if (mode == Mode::MULTI) {
		if (!switchTimer.isStarted()) {
			switchTimer.start();
		}

		if (switchTimer.getTicks() > 100) {
			switchTimer.stop();
			switchTimer.start();
			clip.y = clip.y == 0 ? 15 : 0;
			setClip(clip);
		}
	}
}

void Rocket::render(const flat2d::RenderData* data) const
{
#ifdef DEBUG
	SDL_SetRenderDrawColor(data->getRenderer(), 0xFF, 0x00, 0x00, 0xFF );
	const flat2d::EntityProperties::Areas currentAreas = entityProperties.getCurrentAreas();
	for(auto it = currentAreas.begin(); it != currentAreas.end(); it++) {
		SDL_Rect bounds = (*it).asSDLRect();
		bounds.x = data->getCamera()->getScreenXposFor(bounds.x);
		bounds.y = data->getCamera()->getScreenYposFor(bounds.y);
		SDL_RenderDrawRect( data->getRenderer(), &bounds );
	}
#endif

	Entity::render(data);
}

void Rocket::postRender(const flat2d::GameData *data)
{
	if (!isDead()) {
		setDead(deathTimer.getTicks() > 5000);
	}
}

Rocket::Mode Rocket::getMode() const
{
	return mode;
}

bool Rocket::onCollision(Entity* e, const flat2d::GameData *data)
{
	if (e->getType() == EntityType::BOT) {
		Soldier *soldier = static_cast<Soldier*>(e);
		bool ghostSoldier = soldier->isGhostMode();
		if ((ghostSoldier && mode == GHOST)
				|| (!ghostSoldier && mode == NORMAL)
				|| mode == MULTI)
		{
			soldier->onCollision(this, data);
			SDL_Rect rocketBox = entityProperties.getBoundingBox();
			static_cast<CustomGameData*>(data->getCustomGameData())->getParticleEngine()->createExplosionAt(
					rocketBox.x + static_cast<int>(rocketBox.w/2),
					rocketBox.y + static_cast<int>(rocketBox.h/2));
			data->getMixer()->playEffect(Effects::BANG);
		} else {
			entityProperties.move(data->getDeltatimeMonitor()->getDeltaTime());
			return true;
		}
	}

	setDead(true);
	return true;
}
