#include "Ghost.h"
#include"Simon.h"

CGhost::CGhost(float _x, float _y)
{
	x = _x;
	y = _y;
	enemyType = GHOST;
	ani = CAnimations::GetInstance()->Get(GHOST);
	isActive = 0;
	HP = GHOST_DEFAULT_HP;
	DamageofEnemy = GHOST_DAMAGE;
	isFollow = 1;
	isArmor = 1;
}

CGhost::~CGhost()
{
}

void CGhost::Render()
{
	if (isActive)
	{
		RenderBoundingBox();
		ani->Render(x, y, nx > 0);
	}
}

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	if (!isFollow && GetTickCount() - timeFollow >= GHOST_TIME_FOLLOW_SIMON) {
		isFollow = 1;
	}
	if (isActive)
	{
		if (isFollow)
		{
			float xSimon, ySimon;
			CSimon::GetIntance()->GetPosition(xSimon, ySimon);
			if ((xSimon - x) > 0)
				nx = 1;
			else
				nx = -1;
			vx = nx * GHOST_SPEED_FLY;

			if (ySimon < y)
			{
				vy = -GHOST_SPEED_FLY;
			}
			else if (ySimon > y)
			{
				vy = +GHOST_SPEED_FLY;
			}
			else
				vy = 0;
			

			if (IsContain(GetBBox(), CSimon::GetIntance()->GetBBox()))
			{
				timeFollow = GetTickCount();
				isFollow = 0;
			}
		}
		else
		{
			if (vy == 0)
				vy = GHOST_SPEED_FLY;
		}

		if (y + GHOST_BBOX_HEIGHT > CCamera::GetInstance()->GetRectCam().bottom)
		{
			vy = -vy;
		}

		CGameObject::Update(dt);
		x += dx;
		y += dy;
		if (!IsContain(GetBBox(), CCamera::GetInstance()->GetRectCam()))
		{
			IsDead = 1;
		}
	}
	else
	{
		float xSimon, ySimon;
		CSimon::GetIntance()->GetPosition(xSimon, ySimon);
		if (abs(xSimon - x) >= GHOST_DISTANCE_X_BEGIN_FLY)
		{
			isActive = 1;
		}
	}
}

void CGhost::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + GHOST_BBOX_WIDTH;
	b = y + GHOST_BBOX_HEIGHT;
}

void CGhost::Death(int _hp)
{
	HP -= _hp;
	if (HP <= 0)
	{
		IsDead = true;
	}
	else
	{
		CEnemy::Death(_hp);
	}
}

