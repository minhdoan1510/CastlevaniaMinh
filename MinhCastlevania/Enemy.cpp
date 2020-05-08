#include "Enemy.h"

CEnemy::CEnemy()
{
	objType = ENEMY;
	lifeTime = 0;
	isDamaged = 0;
	HitDamaged = CAnimations::GetInstance()->Get(HIT_DAMAGE);
	HP = ENEMY_DEFAULT_HP;
}

CEnemy::~CEnemy()
{
}

void CEnemy::Render()
{
	if (GetTickCount() - timeHitDamage >= ENEMY_TIME_ANI_HIT)
		isDamaged = 0;
	if (isDamaged)
	{
		HitDamaged->Render(x, y,0);
	}
}

void CEnemy::Death(int _hp)
{

	isDamaged = 1; 
	timeHitDamage = GetTickCount();
}

bool CEnemy::IsActive()
{
	return isActive;
}

int CEnemy::GetDamageofEnemy()
{
	return DamageofEnemy;
}

bool CEnemy::IsArmor()
{
	return isArmor;
}
