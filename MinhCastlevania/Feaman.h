#pragma once
#include"Enemy.h"

#define FEAMAN_BBOX_WIDTH						32	
#define FEAMAN_BBOX_HEIGHT						32
#define FEAMAN_SPEED							0.11f
#define FEAMAN_JUMP_HIGHT						0.45f
#define FEAMAN_JUMP_SHORT						0.12f
#define FEAMAN_JUMP_CLIMB						0.2f
#define FEAMAN_GRAVITY							0.001f 
#define FEAMAN_DEFAULT_HP						1
#define FEAMAN_DISTANCE_X_BEGIN					160
#define FEAMAN_DAMAGE							2
#define FEAMAN_TIME_FOLLOW_SIMON				1500
#define FEAMAN_TIME_PREPARE						1500
#define FEAMAN_TIME_BETWEEN_JUMP				1000
#define FEAMAN_TIME_IDLE						75
#define FEAMAN_DISTANCE_TRICK_JUMP_H			124
#define FEAMAN_DISTANCE_TRICK_JUMP_S			80
#define FEAMAN_DISTANCE_SAFE_WITH_2ND_WEAPON	96

class CFeaman:public CEnemy
{
	bool isFollow;
	bool isPrepare;
	DWORD timeFollow;
	DWORD timeJump;
	int amountJump;
	DWORD timePrepare;
	void Jump(int direct=0, int isJumpHight = -1);
	void SetJumpH(int direct);
	void SetJumpS(int direct);
	bool isJumping;
	bool isIdle;
	DWORD timeIdle;
public:
	CFeaman(float _x, float _y);
	~CFeaman();
	void ClimbJump(int direct);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int hp);
};