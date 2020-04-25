#pragma once
#include"Weapon.h"

#define BOOMERANG_SPEED_ATTACK 1500
#define BOOMERANG_BBOX_WIDTH 28
#define BOOMERANG_BBOX_HEIGHT 28
#define BOOMERANG_FORCE 0.001f
#define BOOMERANG_VTHROW 0.7f


class CBoomerang :public CWeapon
{
public:
	CBoomerang();
	~CBoomerang() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	bool Attack(float _x, float _y, int nx);
	virtual void GetBoundingBox(float& x, float& y, float& framew, float& frameh);
	void Render();
};

