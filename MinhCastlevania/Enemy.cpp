#include "Enemy.h"
#include "Game.h"
#include <algorithm>

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

LPCOLLISIONEVENT CEnemy::SweatAABBx_SafeEnemy(LPGAMEOBJECT coO, float disSafe)
{
	float sl, st, sr, sb;
	float ml, mt, mr, mb;
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	//Xác định khoảng cách an toàn để tránh
	RECT rectWeapon = coO->GetBBox();
	RECT rectThis = this->GetBBox();
	if (rectWeapon.left > rectThis.left)
	{
		sdx -= disSafe;
	}
	else
	{
		sdx += disSafe;
	}

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dx, dy, coO);
	return e;
}

bool CEnemy::IsActive()
{
	return isActive;
}

bool CEnemy::IsBoss()
{
	return isBoss;
}

int CEnemy::GetDamageofEnemy()
{
	return DamageofEnemy;
}

bool CEnemy::IsArmor()
{
	return isArmor;
}

int CEnemy::GetcurrentHP()
{
	return HP;
}

