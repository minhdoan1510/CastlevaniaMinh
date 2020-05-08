#include "MoneyEffect.h"

CMoneyEffect::CMoneyEffect(float _x, float _y, ObjectType type)
{
	x = _x;
	y = _y;
	effectType = type;
	sprite = CSprites::GetInstance()->Get(type);
	lifeTime = GetTickCount();
	numDraw = 0;
	isColisible = 0;
}

CMoneyEffect::~CMoneyEffect()
{

}

void CMoneyEffect::Render()
{
	if (GetTickCount() - lifeTime >= TIME_DEAD_EFFECT)
	{
		IsDead = true;
		return;
	}
	if (numDraw % 20 < 10)
		sprite->Draw(x + PULL_X_EFFECT, y, 255);
	numDraw++;
}

void CMoneyEffect::Update(DWORD dt)
{

}

void CMoneyEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	r = x + 10;
	t = y;
	b = y + 10;
}
