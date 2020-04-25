#pragma once
#include "Weapon.h"

#define	AXE_SPEED 0.5f
#define AXE_GRAVITY 0.003f
#define BOOMERANG_SPEED_ATTACK 500
#define AXE_VFLY 0.7f

#define AXE_BBOX_WIDTH 30
#define AXE_BBOX_HEIGHT 28


#define PULL_Y 20

class CAxe:public CWeapon
{
public:
	CAxe();
	~CAxe() {}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	bool Attack(float _x, float _y, int nx);
	virtual void GetBoundingBox(float& x, float& y, float& framew, float& frameh);
	void Render();
};

