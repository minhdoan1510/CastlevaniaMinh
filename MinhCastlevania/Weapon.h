#pragma once
#include"GameObject.h"

#include"Camera.h"

class CWeapon :public CGameObject
{
protected:
	ObjectType weaponType;
	bool isFinish;
	DWORD lifeTime;
	LPANIMATION ani;
public:
	CWeapon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void Render() = 0;
	bool IsFinish();
	ObjectType GetType();
	virtual bool Attack(float _x,float _y, int nx) = 0;
};

