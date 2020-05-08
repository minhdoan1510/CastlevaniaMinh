#pragma once
#include"GameObject.h"

#define TIME_DEAD_EFFECT 1000
#define PULL_X_EFFECT 10
class CMoneyEffect : public CGameObject
{
protected:
	ObjectType effectType;
	DWORD lifeTime;
	int numDraw;
public:
	CMoneyEffect(float _x, float _y, ObjectType type);
	~CMoneyEffect();
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

