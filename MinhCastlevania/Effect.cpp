#include "Effect.h"

CEffect::CEffect(float _x, float _y,ObjectType _effectType, DWORD _timeEffect , ObjectType _itemHolder)
{
	x = _x;
	y = _y;
	lifeTime = GetTickCount();
	effectType = _effectType;
	ani = CAnimations::GetInstance()->Get(_effectType);
	isDead = 0;
	itemHolder = _itemHolder;
	timeEffect = _timeEffect;
}

CEffect::~CEffect()
{

}

void CEffect::Render()
{
	ani->Render(x, y, 0);
}

void CEffect::Update(DWORD dt)
{
	if (GetTickCount() - lifeTime >= timeEffect)
	{
		isDead = true;
	}
}

bool CEffect::GetDeath()
{
	return isDead;
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
