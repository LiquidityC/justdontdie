#include "ResourceLoader.h"
#include "SoundMappings.h"

bool ResourceLoader::loadSoundEffects(flat2d::GameData *gd)
{
	flat2d::Mixer *mixer = gd->getMixer();

	if (!mixer->loadEffect(Effects::SPLAT, "resources/sound/splat.wav")) {
		return false;
	} else if (!mixer->loadEffect(Effects::SHATTER, "resources/sound/shatter.wav")) {
		return false;
	} else if (!mixer->loadEffect(Effects::BANG, "resources/sound/bang.wav")) {
		return false;
	} else if (!mixer->loadEffect(Effects::JUMP, "resources/sound/jump.wav")) {
		return false;
	}
	return true;
}

bool ResourceLoader::loadMusic(flat2d::GameData *gd)
{
	flat2d::Mixer *mixer = gd->getMixer();

	if (!mixer->loadMusic(Music::MAIN, "resource/sound/axayacatl.ogg")) {
		return false;
	}
	return true;
}
