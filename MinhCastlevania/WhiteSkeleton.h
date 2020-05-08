#pragma once
#include "Enemy.h"
#include "WhiteSkeletonWeapon.h"
#include "Brick.h"

#define WHITESKELETON_BBOX_WIDTH			32
#define WHITESKELETON_BBOX_HEIGHT			63
#define WHITESKELETON_DAMAGE				3
#define WHITESKELETON_GRAVITY				0.001f
#define WHITESKELETON_SPEED_WALKING			0.1f	//0.1f
#define WHITESKELETON_VJUMP					0.21f	//0.34f//0.34f//test
#define WHITESKELETON_DEFAULT_HP			1
#define WHITESKELETON_DISTANCE_MOVE			40
#define WHITESKELETON_DISTANCE_WITH_SIMON	85
#define WHITESKELETON_DISTANCE_FOLLOW_SIMON	64
#define WHITESKELETON_HEIGHT_MAX_JUMP		66
#define WHITESKELETON_WIDTH_MAX_JUMP		66
#define WHITESKELETON_SPEED_ATTACK			1000
#define WHITESKELETON_SPEED_MULTI_ATTACK	200
#define WHITESKELETON_DISTANCE_ATTACK		128

class CWhiteSkeleton :public CEnemy
{
	vector<CWhiteSkeletonWeapon*> weapon;
	bool isJumping;
	bool isAttack;
	float x_SimonBackup;
	float x_Simon;
	float x_backup, x_backupDistanceWithSimon;
	void Jump(int direct);
	void ChangeDirect();
	CBrick* FilterBrickCanJump(D3DXVECTOR2 _point,int _nx, vector<CGameObject*>* _bricks);
	float distance;
	DWORD lTimeAttack;
	DWORD lTimeMultiAttack;
public:
	CWhiteSkeleton(float _x, float _y);
	~CWhiteSkeleton();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Death(int hp);
	void Attack();
	int GetDamage();
};