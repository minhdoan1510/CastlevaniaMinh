#pragma once
#include"Enemy.h"

#define PHANTOMBAT_TIME_FOLLOW_SIMON 4000
#define PHANTOMBAT_BBOX_1_WIDTH		63
#define PHANTOMBAT_BBOX_1_HEIGHT	46
#define PHANTOMBAT_BBOX_2_WIDTH		95
#define PHANTOMBAT_BBOX_2_HEIGHT	46
#define PHANTOMBAT_DISTANCE_ACTIVE	32
#define PHANTOMBAT_FLY_SLOW			0.05f
#define PHANTOMBAT_FLY_FAST			0.3f
#define PHANTOMBAT_FLY_VY_PARABOL(a) { (double)((double)3/128)*pow(a,2) - (double)3/64 }
#define PHANTOMBAT_STATE_IDLE			28100
#define PHANTOMBAT_STATE_ATTACK			28200
#define PHANTOMBAT_STATE_ATTACK_WEAPON	28300
#define PHANTOMBAT_STATE_APPROACH		28400
#define PHANTOMBAT_TIME_IDLE						3000
#define PHANTOMBAT_DISTANCE_NEED_APPROACH			256
#define PHANTOMBAT_DISTANCE_FLY_MAX_WITH_SIMON		128
#define PHANTOMBAT_DISTANCE_FLY_ATTACK_WITH_SIMON	64
#define PHANTOMBAT_VY_APPROACH						0.02f

class CPhantomBat:public CEnemy
{
	bool isActive;
	bool isFollow;
	DWORD timeFollow;
	DWORD timeIdle;
	int statePhantomBat;
	bool isMovePhantomBat;
	float yBackupMovePhantomBat;
	float CalcSpeed();
	void MovePhantomBat(int direct, int isApproach);
	void UpdateMovePhantomBat(int direct);
public:
	CPhantomBat(float _x, float _y);
	~CPhantomBat();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int _hp);
};