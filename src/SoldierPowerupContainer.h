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
		Soldier *soldier = nullptr;
		bool ghostMode = false;
		bool bulletMode = false;

	public:
		explicit SoldierPowerupContainer(Soldier *s) : soldier(s) { }
		bool isGhostMode() const;
		bool isBulletMode() const;

		void setMode(Powerup);
};

#endif // SOLDIERPOWERUPCONTAINER_H_
