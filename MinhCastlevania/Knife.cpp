#include "Knife.h"
#include "Trigger.h"
#include "Sound.h"

#define MIN_Y_ATTACK 18

CKnife::CKnife()
{
	ani = CAnimations::GetInstance()->Get(KNIFE);
	objType = weaponType = KNIFE;
}

void CKnife::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (GetTickCount64() - lifeTime > KNIFE_TIME_LIFE)
		isFinish = true;
	if (nx > 0)
		vx = KNIFE_SPEED;
	else
		vx = -KNIFE_SPEED;

	CGameObject::Update(dt);

	if (isFinish) { vx = 0; vy = 0; return; }
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<CGameObject*>* colliable_objectsNonTrigger = new vector<CGameObject*>(); // filter list non-Triggger

	for (int i = 0; i < colliable_objects->size(); i++)
		if (!dynamic_cast<CTrigger*>(colliable_objects->at(i)))
		{
			colliable_objectsNonTrigger->push_back(colliable_objects->at(i));
		}

	CalcPotentialCollisions(colliable_objectsNonTrigger, coEvents);
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

		isFinish = true;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			if (dynamic_cast<CEnemy*>(e->obj))
				static_cast<CEnemy*>(e->obj)->Death(GetDamage());
			e->obj->Death();
			CSound::GetInstance()->play("Hit",0,1);
			
		}
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

}

bool CKnife::Attack(float _x, float _y, int _nx)
{
	if (lifeTime == 0 || GetTickCount64() - lifeTime  > KNIFE_SPEED_ATTACK)
	{
		x = _x;
		y = (_y + PULL_Y < MIN_Y_ATTACK) ? MIN_Y_ATTACK : _y + PULL_Y;
		nx = _nx;
		lifeTime = GetTickCount64();
		isFinish = false;
		CSound::GetInstance()->play("Knife",0,1);
		return true;
	}
	return false;
}

void CKnife::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = this->x;
	t = this->y;
	r = x + 32;
	b = y + 18;
}

void CKnife::Render()
{
	if (isFinish) return;
	ani->Render(x, y, nx > 0);
	RenderBoundingBox();
}

int CKnife::GetDamage()
{
	return KNIFE_DAMAGE;
}
