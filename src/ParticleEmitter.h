#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include <flat/flat.h>
#include <SDL2/SDL.h>

class Particle;

enum ParticleType
{
	BLOOD_PARTICLE,
	GHOST_PARTICLE,
	FIRE_PARTICLE,
	BOOST_PARTICLE
};

class ParticleEmitter
{
	private:
		ParticleType type;
		size_t emitCount = 0;
		bool running = false;

		Particle* createParticleAt(int x, int y);

	public:
		explicit ParticleEmitter(ParticleType t) : type(t) { }

		void emit(const flat2d::GameData*, SDL_Rect area);

		void setEmissionCount(size_t count);
		void setRunning(bool running);
}; // ParticleEmitter

#endif // PARTICLEEMITTER_H_
