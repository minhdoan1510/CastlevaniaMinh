#pragma once
#include "Enemy.h"

#define BLACKKNIGHT_BBOX_WIDTH			32	
#define BLACKKNIGHT_BBOX_HEIGHT			64
#define BLACKKNIGHT_GRAVITY				0.005f 
#define BLACKKNIGHT_SPEED_WALKING		0.05f
#define BLACKKNIGHT_DEFAULT_HP			3
#define BLACKKNIGHT_DISTANCE_MIN		64
#define BLACKKNIGHT_DISTANCE_MAX		110
#define BLACKKNIGHT_DAMAGE				2
#define BLACKKNIGHT_Y_ATTACK_SIMON		32

class CBlackKnight:public CEnemy
{
private:
	float x_backup;
	int distance;
	bool isAttack;

public:
	CBlackKnight(float x, float y);
	~CBlackKnight();
	void Render();
	void ChangeDirect();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int hp);
};

