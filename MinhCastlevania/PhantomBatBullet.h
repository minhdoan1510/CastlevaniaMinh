#pragma once
#include"Weapon.h"
#include "Simon.h"

#define SPEED_MOVE		 0.1f
#define DAMAGE_BULLET	 2
#define BULLET_BBOX		 D3DXVECTOR2(13,12)

class CPhantomBatBullet : CWeapon
{
public:
	CPhantomBatBullet(float _x, float _y);
	void SetVectorMove(float _nvx,float _nvy);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	bool Attack(float _x, float _y, int nx);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	int GetDamage();
	bool IsFinish();
};

