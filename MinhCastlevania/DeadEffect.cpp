#include "DeadEffect.h"

CDeadEffect::CDeadEffect(float _x, float _y, ObjectType _effectType, DWORD _timeEffect, ObjectType _itemHolder):CEffect(_x, _y, _effectType, _timeEffect, _itemHolder)
{
	ani = CAnimations::GetInstance()->Get(_effectType);
	lifeTime = GetTickCount();
}

CDeadEffect::~CDeadEffect()
{
}

void CDeadEffect::Render()
{
	ani->Render(x, y, 0);
}

void CDeadEffect::Update(DWORD dt)
{
	CEffect::Update(dt);
}
