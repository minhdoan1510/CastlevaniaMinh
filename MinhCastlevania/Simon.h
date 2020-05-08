#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Whip.h"


#define SIMON_BBOX_WIDTH			60
#define SIMON_BBOX_HEIGHT			63
#define SIMON_BBOX_SITTING_HEIGHT	45
#define SIMON_BBOX_JUMPING_HEIGHT	45

#define PULL_UP_SIMON_SITTING 18.0f 

#define SIMON_GRAVITY			0.005f 
#define SIMON_GRAVITY_JUMPING	0.001f 
#define SIMON_GRAVITY_HURTING	0.001f

#define SIMON_WALKING_SPEED 0.12f 
#define SIMON_SPEED_ONSTAIR 0.052f 
#define SIMON_VJUMP			0.36f
#define SIMON_SPEED_AUTO_GO 0.09f 
//#define SIMON_SPEED_ATTACK_SECOND_WEAPON 1000


#define SIMON_ANI_DEATH			-900000
#define SIMON_ANI_INJURED		-910000

#define SIMON_ANI_IDLE					1000
#define SIMON_ANI_WALKING				1010
#define SIMON_ANI_JUMPING				1020
#define SIMON_ANI_SITTING				1030
#define SIMON_ANI_STANDING_ATTACKING	1040
#define SIMON_ANI_SITTING_ATTACKING		1050
#define SIMON_ANI_STAIR_STANDING_UP		1060
#define SIMON_ANI_STAIR_UP				1061
#define SIMON_ANI_STAIR_STANDING_DOWN	1070
#define SIMON_ANI_STAIR_DOWN			1071
#define SIMON_ANI_STAIR_UP_ATTACKING	1080
#define SIMON_ANI_STAIR_DOWN_ATTACKING	1090
#define SIMON_ANI_ENTER_GAME			1100

#define SIMON_DEFAULT_HP	16
#define SIMON_DEFAULT_LIFE	3
#define SIMON_DEFAULT_HEART	5

#define SIMON_UNTOUCHABLE_TIME	2000 
#define TIME_FREEZE_MAX			500
#define TIME_ANI_DEATH			2000
#define TIME_ONCE_STAIR			300

#define TIME_SIMON_ATTACK 360



class CSimon : public CGameObject
{
	static CSimon* intance;
	int isuntouchable;
	DWORD untouchable_start;
	DWORD timeAniDeadth;

	ObjectType weaponcurrent;
	ObjectType secondaryweaponcurrent;
	unordered_map<int, CWeapon*> listWeapon;

	int heart;
	int lifeSimon;
	int HPSimon;
	int amountSecondWeapon;

	bool isAutoGo;
	float AutoGo_X_Distance;
	float AutoGo_X_Backup;
	int DirectEnd;

	int aniState;
	bool isAttacking;
	bool isWalking;
	bool isFreeze;
	bool isJumping;
	bool isSitting;
	bool isWantOnBottomstair;
	bool isWantOnTopstair;
	bool isInjured;
	bool flagRenderUntouch;
	bool isEnterGameIntro;

	bool isOnStair;
	int directOnStair;
	int numberStair;
	bool isProcessStair;
	float h_StepStair;
	bool isContainTopTrigger;
	bool isContainBottomTrigger;
	DWORD timeOnceStair;
	float y_OnStair;

	bool isAniDead;

	DWORD TimeFreeze;
	
	float x_backup;
	float y_backup;
	int nx_backup;

	DWORD lifetimeAttack;

	vector<CGameObject*> objAddAfterUpdate;

	void clearListSecondWeapon();
public:
	CSimon();
	static CSimon* GetIntance();
	void SetPosition(float _x, float _y);
	void SetFinish(bool _i);
	int GetNx() { return nx; }
	void ResetPositionBackup() { x = x_backup; y = y_backup; nx = nx_backup; }
	void DeathSimon();
	bool IsDeading() { return isAniDead; }
	int GetDirectStair() { return directOnStair; }
	bool IsOnStair() { return isOnStair; }
	void SetAutoGo(float _AutoGo_X_Distance, int DirectBegin, int _DirectEnd);
	bool IsAutoGo();
	bool IsProcessStair() { return isProcessStair; }
	void AddHeart(int n) { heart += n; }
	int GetHeart() { return heart; }
	int GetLifeSimon() { return lifeSimon; }
	int GetHPSimon() { return HPSimon; }
	int GetAmount2ndWeapon() { return listWeapon.size() - 1; }
	vector<CGameObject*> GetAbjAddAfterUpdate() { return objAddAfterUpdate;	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void StartUntouchable() { isuntouchable = 1; untouchable_start = GetTickCount64(); }
	bool IsInjured() {return isInjured;}
	ObjectType GetSecondWeapon() { return secondaryweaponcurrent; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Left();
	void Right();
	void Go();
	void Sit();
	void ResetSit();
	void Jump();
	void Stop();
	void EnterIntroGameState(bool isEnter);
	void SetFreeze(bool _isFreeze);
	bool IsFreeze();
	void ResetSimon();
	void SetInjured(int damage,int direct);
	void SetStateAttacked(int direct);
	void Attack(bool isMainWeapon);
	void WantUpOnStair();
	void CollisionWithItem(ObjectType _type);
	void CollisionWithEnemy(CGameObject* obj);
};