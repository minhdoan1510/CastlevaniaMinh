#include "Boomerang.h"
#include "Simon.h"
#include "Item.h"

CBoomerang::CBoomerang()
{
	ani = new CAnimation(*CAnimations::GetInstance()->Get(BOOMERANG));
	objType = weaponType = BOOMERANG;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (isFinish) return;
	if(nx>0)
		vx += -BOOMERANG_FORCE * dt;
	else
		vx += BOOMERANG_FORCE * dt;
	CGameObject::Update(dt);
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
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;

		x += dx;
		y += dy;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;
		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			if (dynamic_cast<CSimon*>(e->obj))
			{
				isFinish = true;
			}
			if (dynamic_cast<CItem*>(e->obj)) continue;
			e->obj->Death();
		}
	}

}

bool CBoomerang::Attack(float _x, float _y, int _nx)
{
	if (lifeTime == 0 || GetTickCount64() - lifeTime > BOOMERANG_SPEED_ATTACK)
	{
		lifeTime = GetTickCount64();
		isFinish = false;
		x = _x;
		y = (_y + PULL_Y < 18) ? 18 : _y + PULL_Y;
		nx = _nx;
		if (_nx > 0)
			vx = BOOMERANG_VTHROW;
		else
			vx = -BOOMERANG_VTHROW;
		return true;
	}
	return false;
}

void CBoomerang::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BOOMERANG_BBOX_WIDTH;
	b = y + BOOMERANG_BBOX_HEIGHT;
}

void CBoomerang::Render()
{
	if (isFinish) return;
	RenderBoundingBox();
	ani->Render(x, y, nx < 0);
}
