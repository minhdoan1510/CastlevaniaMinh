#include "Effect.h"

CEffect::CEffect(float _x, float _y, ObjectType type)
{
	x = _x;
	y = _y;
	effectType = type;
	sprite = CSprites::GetInstance()->Get(type);
	lifeTime = GetTickCount();
	numDraw = 0;
}

CEffect::~CEffect()
{

}

void CEffect::Render()
{
	if (GetTickCount() - lifeTime >= TIME_DEAD_EFFECT)
	{
		IsDead == true;
		return;
	}
	if (numDraw % 20 < 10)
		sprite->Draw(x + 10, y, 255);
	numDraw ++;
}

void CEffect::Update(DWORD dt)
{

}

void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
