#pragma once
#include"Enemy.h"

#define GHOST_BBOX_WIDTH				32	
#define GHOST_BBOX_HEIGHT				32
#define GHOST_SPEED_FLY					0.08f
#define GHOST_DEFAULT_HP				3
#define GHOST_DISTANCE_X_BEGIN_FLY		224
#define GHOST_DAMAGE					2
#define GHOST_TIME_FOLLOW_SIMON			500


class CGhost:public CEnemy
{
	float yFly;
	bool isFollow;
	DWORD timeFollow;
public:
	CGhost(float _x, float _y);
	~CGhost();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int hp);
};

