#include "Gunpowder.h"
#include "Item.h"

CGunpowder::CGunpowder()
{
	isCollision = 0;
	ani = new CAnimation(*(CAnimations::GetInstance()->Get(GUNPOWDER)));
	weaponType = GUNPOWDER;
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
	if (ani->GetDoneFrameFinal())
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
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			if (!dynamic_cast<CItem*>(e->obj))
			{
				e->obj->Death();
				isCollision = 1;
				y -= 10;
			}
		}
	}
}

bool CGunpowder::Attack(float _x, float _y, int _nx)
{
	if (lifeTime == 0 || GetTickCount64() - lifeTime > GUNPOWDER_SPEED_ATTACK)
	{
		ani->ResetFarmeCurrent();
		lifeTime = GetTickCount64();
		isFinish = false;
		isCollision = 0;
		x = _x;
		y = (_y + PULL_Y < 18) ? 18 : _y + PULL_Y;
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