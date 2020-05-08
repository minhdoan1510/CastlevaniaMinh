#pragma once
#include "Weapon.h"

#define KNIFE_SPEED_ATTACK 500
#define KNIFE_SPEED .6f
#define KNIFE_TIME_LIFE 6000

#define KNIFE_DAMAGE 1

#define PULL_Y 20

class CKnife:public CWeapon
{
public:
	CKnife();
	~CKnife() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	bool Attack(float _x, float _y, int nx);
	virtual void GetBoundingBox(float& x, float& y, float& framew, float& frameh);
	void Render();
	int GetDamage();

};

