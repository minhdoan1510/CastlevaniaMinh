#pragma once
#include"GameObject.h"
#include"Camera.h"
#include "Enemy.h"

class CWeapon :public CGameObject
{
protected:
	ObjectType weaponType;
	bool isFinish;
	DWORD lifeTime;
	bool isUsedOnceTime;
	bool isCollision;
public:
	CWeapon();
	bool IsFinish();
	ObjectType GetType();
	void CollisionWithEnemy(CEnemy* e, int damage);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void Render() = 0;
	virtual bool Attack(float _x, float _y, int nx) = 0;
	virtual int GetDamage() =0;
	void CollisionWithObjects(vector<CGameObject*>* objects);
};

