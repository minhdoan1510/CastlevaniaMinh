#include "MoneyEffect.h"

CMoneyEffect::CMoneyEffect(float _x, float _y, ObjectType _type):CEffect(_x,_y,_type, TIME_MONEY_EFFECT)
{
	ani = CAnimations::GetInstance()->Get(_type);
	lifeTime = GetTickCount();
}

CMoneyEffect::~CMoneyEffect()
{

}

void CMoneyEffect::Render()
{
	if (GetTickCount() - lifeTime >= TIME_DEAD_EFFECT)
	{
		isFinish = true;
		return;
	}
	ani->Render(x,y,0);
}

void CMoneyEffect::Update(DWORD dt)
{

}

