#pragma once
#include"Enemy.h"

#define ZOMBIE_BBOX_WIDTH				31
#define ZOMBIE_BBOX_HEIGHT				64
#define ZOMBIE_SPEED_WALKING			0.1f
#define ZOMBIE_GRAVITY					0.05f 
#define ZOMBIE_DEFAULT_HP				1
#define ZOMBIE_DAMAGE					2

class CZombie :public CEnemy
{
	bool isContainCam;
public:
	CZombie(float _x, float _y);
	~CZombie();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int hp);
};

