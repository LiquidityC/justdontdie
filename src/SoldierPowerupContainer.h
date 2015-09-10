#ifndef SOLDIERPOWERUPCONTAINER_H_
#define SOLDIERPOWERUPCONTAINER_H_

class Soldier;

enum Powerup
{
	NORMAL,
	GHOST,
	BULLET
};

class SoldierPowerupContainer
{
	private:
		bool ghostMode = false;
		bool bulletMode = false;

	public:
		bool isGhostMode() const;
		bool isBulletMode() const;

		void setMode(Powerup);
};

#endif // SOLDIERPOWERUPCONTAINER_H_
