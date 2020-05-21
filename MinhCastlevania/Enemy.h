#pragma once
#include "GameObject.h"

#define ENEMY_DEFAULT_HP 1
#define ENEMY_TIME_ANI_HIT 200

class CEnemy:public CGameObject
{
protected:
	int HP;
	DWORD lifeTime;
	DWORD timeHitDamage;
	ObjectType enemyType;
	LPANIMATION HitDamaged;
	bool isDamaged;
	int DamageofEnemy;
	bool isActive;
	bool isArmor;
public:
	CEnemy();
	~CEnemy();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL) = 0;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
	virtual void Death(int _hp);
	LPCOLLISIONEVENT SweatAABBx_SafeEnemy(LPGAMEOBJECT coO, float disSafe);
	bool IsActive();
	int GetDamageofEnemy();
	bool IsArmor();
};

