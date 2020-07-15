#include "Gunpowder.h"
#include "Item.h"
#include "Sound.h"
#include "Ghost.h"
#include "Brick.h"

#define MIN_Y_ATTACK 18
CGunpowder::CGunpowder()
{
	isCollision = 0;
	ani =CAnimations::GetInstance()->Get(GUNPOWDER);
	weaponType = GUNPOWDER;
	isFinish = true;
}

CGunpowder::~CGunpowder()
{
}

void CGunpowder::Render()
{
	if (isFinish) return;
	if (!isCollision)
	{
		RenderBoundingBox();
		ani->RenderFrame(0, x, y,0);
	}
	else
	{
		ani->Render(x, y, 0);
	}
}

void CGunpowder::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFinish) return;
	if (GetTickCount64() - timeFire > GUNPOWDER_TIME_ATTACK && timeFire != 0)
	{
		isFinish = 1;
		return;
	}

	if (!isCollision)
	{

		if (nx > 0)
			vx = GUNPOWDER_SPEED;
		else
			vx = -GUNPOWDER_SPEED;

		CGameObject::Update(dt);
		vy += GUNPOWDER_GRAVITY * dt;
	}
	else
	{
		dy = dx = vy = vy = 0;
	}
	


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 

		bool flag = 0;
		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			if (!dynamic_cast<CItem*>(e->obj))
			{
				if (dynamic_cast<CEnemy*>(e->obj) && !dynamic_cast<CGhost*>(e->obj))
					static_cast<CEnemy*>(e->obj)->Death(GetDamage());
				else
					e->obj->Death();
				if (dynamic_cast<CBrick*>(e->obj))
					flag = 1;
				CSound::GetInstance()->play("Hit", 0, 1);
				y -= 10;
				timeFire = GetTickCount();
			}
		}
		isCollision = flag;
		if (flag)
		{
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;
			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;
		}
	}
}

bool CGunpowder::Attack(float _x, float _y, int _nx)
{
	if ((lifeTime == 0 || GetTickCount64() - lifeTime > GUNPOWDER_SPEED_ATTACK)&& isFinish == true)
	{
		timeFire = 0;
		ani->ResetFarmeCurrent();
		lifeTime = GetTickCount64();
		isFinish = false;
		isCollision = 0;
		x = _x;
		y = (_y + PULL_Y < MIN_Y_ATTACK) ? MIN_Y_ATTACK : _y + PULL_Y;
		nx = _nx;
		vy = -GUNPOWDER_VFLY;
		return true;
	}
	return false;
}

void CGunpowder::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = this->x;
	t = this->y;
	r = x + GUNPOWDER_BBOX_WIDTH;
	b = y + GUNPOWDER_BBOX_HEIGHT;
}

int CGunpowder::GetDamage()
{
	return GUNPOWDER_DAMAGE;
}
