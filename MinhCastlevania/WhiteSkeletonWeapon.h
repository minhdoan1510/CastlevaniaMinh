#pragma once
#include "Weapon.h"

#define WHITESKELETONWEAPON_BBOX_WIDTH 32
#define WHITESKELETONWEAPON_BBOX_HEIGHT 32
#define	WHITESKELETONWEAPON_SPEED 0.1f
#define WHITESKELETONWEAPON_GRAVITY 0.001f
#define WHITESKELETONWEAPON_SPEED_ATTACK 500
#define WHITESKELETONWEAPON_VFLY 0.4f
#define WHITESKELETONWEAPON_DAMAGE 3

class CWhiteSkeletonWeapon:public CWeapon
{
	bool isStart;
public:
	CWhiteSkeletonWeapon(float _x, float _y, int _nx);
	~CWhiteSkeletonWeapon();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	bool Attack(float x, float y, int nx);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetDamage();
	bool IsStart();
	void Start(float _x, float _y, int _nx);
};

