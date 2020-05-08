#pragma once
#include"Enemy.h"

#define BAT_BBOX_WIDTH				32	
#define BAT_BBOX_HEIGHT				32
#define BAT_SPEED_FLY				0.2f
#define BAT_DEFAULT_HP				1
#define BAT_MAX_FLY_DOWN			96
#define BAT_DISTANCE_X_BEGIN_FLY	180
#define BAT_DAMAGE					2

class CBat:public CEnemy
{
	float yBackup, yFly;
public:
	CBat(float _x, float _y);
	~CBat();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int hp);
};

