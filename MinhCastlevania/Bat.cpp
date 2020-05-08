#include "Bat.h"
#include "Simon.h"

CBat::CBat(float _x,float _y)
{
	x = _x;
	yBackup = y = _y;
	enemyType = BAT;
	vy = BAT_SPEED_FLY;
	sprite = CSprites::GetInstance()->Get(BAT_SLEEP);
	ani = CAnimations::GetInstance()->Get(BAT);
	isActive = 0;
	DamageofEnemy = BAT_DAMAGE;
	isArmor = 0;
}

CBat::~CBat()
{
}

void CBat::Render()
{
	RenderBoundingBox();
	if (!isActive)
	{
		sprite->Draw(x, y);
	}
	else {
		ani->Render(x, y, nx > 0);
	}
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	if (isActive)
	{
		CGameObject::Update(dt);
		if (y >= yFly)
		{
			y = yFly;
			vy = 0;
		}
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
		CSimon::GetIntance()->GetPosition(xSimon,ySimon);
		if ( ySimon -y  <= BAT_MAX_FLY_DOWN && abs(xSimon - x) <= BAT_DISTANCE_X_BEGIN_FLY)
		{
			yFly = ySimon;
			if (xSimon - x > 0)
			{
				nx = 1;
			}
			else
			{
				nx = -1;
			}
			vx = nx*BAT_SPEED_FLY;
			isActive = 1;
		}
	}
}

void CBat::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BAT_BBOX_WIDTH;
	b = y + BAT_BBOX_HEIGHT;
}

void CBat::Death(int _hp)
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
