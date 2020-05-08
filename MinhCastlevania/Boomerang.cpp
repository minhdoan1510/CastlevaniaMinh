#include "Boomerang.h"
#include "Simon.h"
#include "Item.h"
#include "Brick.h"

#define MIN_Y_ATTACK 18

CBoomerang::CBoomerang()
{
	ani = CAnimations::GetInstance()->Get(BOOMERANG);
	objType = weaponType = BOOMERANG;
	isUsedOnceTime = 0;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (isFinish) return;
	if (nx > 0)
	{
		vx += -BOOMERANG_FORCE * dt;
	}
	else
	{
		vx += BOOMERANG_FORCE * dt;
	}
	if (GetTickCount() - lifeTime > BOOMERANG_MAX_LIFE)
	{
		isFinish = true;
	}


	CGameObject::Update(dt);
	if (isFinish) { vx = 0; vy = 0; return; }

	//CollisionWithObjects(colliable_objects);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	
	vector<LPGAMEOBJECT>* _objects = new vector<LPGAMEOBJECT>();
	for (int i = 0; i < colliable_objects->size(); i++) {
		//if (dynamic_cast<CBrick*> (colliable_objects->at(i)))
			_objects->push_back(colliable_objects->at(i));
	}
	_objects->push_back(CSimon::GetIntance());

	CalcPotentialCollisions(_objects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, _nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, _nx, ny);

		x += dx;
		y += dy;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			if (dynamic_cast<CSimon*>(e->obj))
			{
				isFinish = true;
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (nx_backup == ((vx>0)?1:-1)) //block 1 chiều boomerang
					vx = 0;
				else {
					DebugOut(L"");
				}
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
				static_cast<CEnemy*>(e->obj)->Death(GetDamage());
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
		y = (_y + PULL_Y < MIN_Y_ATTACK) ? MIN_Y_ATTACK : _y + PULL_Y;
		nx = _nx;
		nx_backup = _nx;
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
	if (nx != vx / abs(vx))
		ani->Render(x, y, 1);
	else
		ani->Render(x, y, 0);
}

int CBoomerang::GetDamage()
{
	return BOOMERANG_DAMAGE;
}
