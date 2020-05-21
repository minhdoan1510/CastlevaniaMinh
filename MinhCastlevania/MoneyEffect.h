#pragma once
#include"Effect.h"

#define TIME_DEAD_EFFECT	1000
#define PULL_X_EFFECT		10
#define TIME_MONEY_EFFECT	2500

class CMoneyEffect : public CEffect
{
private:
	ObjectType effectType;
	DWORD lifeTime;
	int numDraw;
public:
	CMoneyEffect(float _x, float _y, ObjectType type);
	~CMoneyEffect();
	void Render();
	void Update(DWORD dt);
};

