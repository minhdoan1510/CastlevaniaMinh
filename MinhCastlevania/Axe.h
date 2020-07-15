#pragma once
#include "Weapon.h"

#define	AXE_SPEED 0.2f
#define AXE_GRAVITY 0.002f
#define AXE_SPEED_ATTACK 500
#define AXE_VFLY 0.7f

#define AXE_BBOX_WIDTH 30
#define AXE_BBOX_HEIGHT 28

#define AXE_DAMAGE 1

#define PULL_Y 20

class CAxe:public CWeapon
{
public:
	CAxe();
	~CAxe() {}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	bool Attack(float _x, float _y, int nx);
	void GetBoundingBox(float& x, float& y, float& framew, float& frameh);
	void Render(); 
	int GetDamage();
};

