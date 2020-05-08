#pragma once
#include"Weapon.h"

#define BOOMERANG_SPEED_ATTACK 1500
#define BOOMERANG_BBOX_WIDTH 28
#define BOOMERANG_BBOX_HEIGHT 28
#define BOOMERANG_FORCE 0.00025f
#define BOOMERANG_VTHROW .35f//0.7f
#define BOOMERANG_MAX_LIFE 5000

#define BOOMERANG_DAMAGE 1

class CBoomerang :public CWeapon
{
	int nx_backup;
public:
	CBoomerang();
	~CBoomerang() {}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	bool Attack(float _x, float _y, int nx);
	void GetBoundingBox(float& x, float& y, float& framew, float& frameh);
	void Render();
	int GetDamage();
};

