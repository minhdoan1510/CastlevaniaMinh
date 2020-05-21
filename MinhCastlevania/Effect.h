#pragma once
#include "Animations.h"
#define TIME_DEAD_EFFECT 1000
#define PULL_X_EFFECT 10

class CEffect
{
protected:
	ObjectType effectType;
	DWORD lifeTime;
	float x, y;
	ObjectType itemHolder;
	DWORD timeEffect;
	bool isDead;
	LPANIMATION ani;
	LPSPRITE sprite;
	float isFinish;
public:
	CEffect(float _x, float _y, ObjectType _effectType, DWORD _timeEffect = 5000, ObjectType _itemHolder = Null);
	~CEffect();
	virtual void Render()=0;
	virtual void Update(DWORD dt);
	bool IsFinish();
	ObjectType GetItemHolder();
	void GetPosition(float &x, float &y);
};