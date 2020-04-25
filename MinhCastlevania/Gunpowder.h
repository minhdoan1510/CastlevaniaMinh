#pragma once
#include "Weapon.h"

#define GUNPOWDER_SPEED 0.13f
#define GUNPOWDER_GRAVITY 0.0005f
#define GUNPOWDER_SPEED_ATTACK 1500
#define GUNPOWDER_VFLY 0.2f

#define GUNPOWDER_BBOX_WIDTH 16
#define GUNPOWDER_BBOX_HEIGHT 16

class CGunpowder:public CWeapon
{
	bool isCollision;
public:
	CGunpowder();
	~CGunpowder();

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	bool Attack(float x, float y, int nx);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};