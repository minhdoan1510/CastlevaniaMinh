#include "Effect.h"

CEffect::CEffect(float _x, float _y,ObjectType _effectType, DWORD _timeEffect , ObjectType _itemHolder)
{
	x = _x;
	y = _y;
	lifeTime = GetTickCount();
	effectType = _effectType;
	itemHolder = _itemHolder;
	timeEffect = _timeEffect;
	isFinish = 0;
}

CEffect::~CEffect()
{

}

void CEffect::Update(DWORD dt)
{
	if (GetTickCount() - lifeTime >= timeEffect)
	{
		isFinish = true;
	}
}

bool CEffect::IsFinish()
{
	return isFinish;
}

ObjectType CEffect::GetItemHolder()
{
	return itemHolder;
}

void CEffect::GetPosition(float& _x, float& _y)
{
	_x = x;
	_y = y;
}
