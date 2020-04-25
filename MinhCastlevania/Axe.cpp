#include "Axe.h"
#include "Item.h"
#include "Brick.h"

CAxe::CAxe()
{
	ani = new CAnimation(*CAnimations::GetInstance()->Get(AXE));
	objType = weaponType = AXE;
}

void CAxe::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (nx > 0)
		vx = AXE_SPEED;
	else
		vx = -AXE_SPEED;

	CGameObject::Update(dt);
	vy += AXE_GRAVITY * dt;

	if (isFinish) { vx = 0; vy = 0; return; }

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(colliable_objects, coEvents);
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
		bool isFlag = false;
		for (size_t i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			if (!dynamic_cast<CItem*>(e->obj))
			{
				e->obj->Death();
				isFlag = true;
			}
		}
		if(isFlag)
			isFinish = true;
	}

}

bool CAxe::Attack(float _x, float _y, int _nx)
{
	if (lifeTime == 0 || GetTickCount64() - lifeTime > BOOMERANG_SPEED_ATTACK)
	{
		lifeTime = GetTickCount64();
		isFinish = false;

		x = _x;
		y = (_y + PULL_Y < 18) ? 18 : _y + PULL_Y;
		nx = _nx;
		vy = -AXE_VFLY;

		return true;
	}
	return false;
}

void CAxe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = this->x;
	t = this->y;
	r = x + AXE_BBOX_WIDTH;
	b = y + AXE_BBOX_HEIGHT;
}

void CAxe::Render()
{
	if (isFinish) return;
	RenderBoundingBox();
	ani->Render(x, y, nx < 0);
}
