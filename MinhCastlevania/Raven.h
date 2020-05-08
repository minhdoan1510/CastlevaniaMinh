#pragma once
#include"Enemy.h"


#define RAVEN_BBOX_WIDTH				32	
#define RAVEN_BBOX_HEIGHT				32
#define RAVEN_SPEED_Y_FLY				0.08f
#define RAVEN_SPEED_X_FLY				0.03f
#define RAVEN_SPEED_FLY_ATTACKING		0.2f
#define RAVEN_DEFAULT_HP				1
#define RAVEN_DISTANCE_X_BEGIN_FLY		160
#define RAVEN_DAMAGE					2
#define RAVEN_TIME_FOLLOW_SIMON			500
#define RAVEN_DISTANCE_FLY_SIMON		80
#define RAVEN_DISTANCE_TRICK_SIMON		100

class CRaven:public CEnemy
{

	float xSimonBackup, ySimonBackup, yFly;
	bool isFollow;
	DWORD timeFollow;
	bool isAttackBottomOrTopofWhip;
	bool Attack;
	bool EndAttack;
	void FlyPoint(float x, float y);
	void UnFollow();
	bool isAutoFly;
	float XCP, YCP, DistanceCheckPoint,nyCP;
public:
	CRaven(float _x, float _y);
	~CRaven();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int hp);
};

