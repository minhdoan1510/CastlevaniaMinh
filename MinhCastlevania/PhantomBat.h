#pragma once
#include"Enemy.h"
#include "PhantomBatBullet.h"

#define PHANTOMBAT_TIME_FOLLOW_SIMON 4000
#define PHANTOMBAT_BBOX_1_WIDTH		63
#define PHANTOMBAT_BBOX_1_HEIGHT	46
#define PHANTOMBAT_BBOX_2_WIDTH		95
#define PHANTOMBAT_BBOX_2_HEIGHT	46
#define PHANTOMBAT_DISTANCE_ACTIVE	32
#define PHANTOMBAT_FLY_SLOW			0.05f
#define PHANTOMBAT_FLY_FAST			0.3f
#define PHANTOMBAT_FLY_VY_PARABOL(vy,dis) (float)(float(vy / 65536) * pow(dis, 2) - (vy / 128) * dis + vy)
#define PHANTOMBAT_FLY_HALF			256
#define PHANTOMBAT_STATE_IDLE			28100
#define PHANTOMBAT_STATE_ATTACK			28200
#define PHANTOMBAT_STATE_ATTACK_WEAPON	28300
#define PHANTOMBAT_STATE_APPROACH		28400
#define PHANTOMBAT_STATE_APPROACH_DOWN	28500
#define PHANTOMBAT_DAMAGE				2
#define PHANTOMBAT_DEFAULT_HP			16

#define PHANTOMBAT_STATE_FLY_1			29100
#define PHANTOMBAT_STATE_FLY_2			29200
#define PHANTOMBAT_STATE_FLY_3			29300

#define PHANTOMBAT_TIME_IDLE						1200
#define PHANTOMBAT_DISTANCE_NEED_APPROACH			256
#define PHANTOMBAT_DISTANCE_FLY_MAX_WITH_SIMON		128
#define PHANTOMBAT_DISTANCE_FLY_ATTACK_WITH_SIMON	64
#define PHANTOMBAT_VY_APPROACH						0.02f
#define PHANTOMBAT_FORCE_VY							0.14f

class CPhantomBat:public CEnemy
{
	int stateFly;
	bool isFollow;
	bool isProcessing;
	DWORD timeFollow;
	DWORD timeIdle;
	int statePhantomBat;
	bool isMovePhantomBat;
	bool isFinishMove;
	float yBackupMovePhantomBat;
	float xBackupMovePhantomBat;
	float CalcSpeed(float _vx);
	int directFly;
	float distanceFlew;
	void MovePhantomBatPal(int direct, bool isApproach);
	void UpdateMovePhantomBat(RECT boxSimon);
	CPhantomBatBullet* bullet;
	//void UpdateMovePhantomBat();
public:
	CPhantomBat(float _x, float _y);
	~CPhantomBat();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int _hp);
	ObjectType GetItemHolder();
};