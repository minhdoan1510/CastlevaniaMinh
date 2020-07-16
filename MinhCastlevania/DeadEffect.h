#pragma once
#include "Effect.h"

class CDeadEffect: public CEffect
{
public:
	CDeadEffect(float _x, float _y, ObjectType _effectType, DWORD _timeEffect, ObjectType _itemHolder=Null);
	~CDeadEffect();
	void Render();
	void Update(DWORD dt);
};

