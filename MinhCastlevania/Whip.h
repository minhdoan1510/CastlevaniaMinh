#pragma once
#include"Weapon.h"

#define WHIP_HEIGHT 16

#define WHIP_1_WIDTH 50
#define WHIP_2_WIDTH 50
#define WHIP_3_WIDTH 80

#define WHIP_1_DAMAGE 1
#define WHIP_2_DAMAGE 2
#define WHIP_3_DAMAGE 2

#define WHIP_BBOX_X 30
#define WHIP_BBOX_Y 20

#define WHIP_SPEED_ATTACK 500

#define X_FRAME_0_RIGHT	23
#define X_FRAME_0_LEFT	80
#define X_FRAME_1_RIGHT 25
#define X_FRAME_1_LEFT	75
#define X_FRAME_2_RIGHT 25
#define X_FRAME_2_LEFT	75

class CWhip : public CWeapon
{
	int leverWhip;
	int whipWidth;
	ObjectType whipType;
	int damage;	
	bool isDamage;
public:
	CWhip();
	~CWhip();
	void SetLever(int lv); 
	int GetLever() { return leverWhip; }
	bool Attack(float _x, float _y, int nx);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetDamage();
};

