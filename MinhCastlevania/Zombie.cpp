#include "Zombie.h"
#include "Simon.h"
#include "Brick.h"

CZombie::CZombie(float _x, float _y)
{
	x = _x;
	y = _y;
	enemyType = ZOMBIE;
	ani = CAnimations::GetInstance()->Get(ZOMBIE)->Clone();
	isActive = 1;
	DamageofEnemy = ZOMBIE_DAMAGE;
	isArmor = 1;
	nx = 0;
	isContainCam = 0;
}

CZombie::~CZombie()
{
}

void CZombie::Render()
{
	RenderBoundingBox();
	ani->Render(x, y, nx == 1);
	CEnemy::Render();
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* _objects)
{
	if (IsDead)
		return;
	if (nx == 0)
	{
		float _xSimon, _ySimon;
		CSimon::GetIntance()->GetPosition(_xSimon, _ySimon);
		if (x < _xSimon)
			nx = 1;
		else
			nx = -1;
	}
	vx = nx * ZOMBIE_SPEED_WALKING;
	vy = dt * ZOMBIE_GRAVITY;
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<CGameObject*>* colliable_objects = new vector<CGameObject*>(); // filter list Brick

	for (int i = 0; i < _objects->size(); i++)
		if (dynamic_cast<CBrick*>(_objects->at(i)))
		{
			colliable_objects->push_back(_objects->at(i));
		}

	CalcPotentialCollisions(colliable_objects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, _nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, _nx, ny);

		x += min_tx * dx + _nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (_nx != 0)
			vx = 0;
		if (ny != 0)
			vy = 0;
	}

	if (!IsContain(CCamera::GetInstance()->GetRectCam(), this->GetBBox()))
	{
		if (isContainCam)
			this->Death(ZOMBIE_DEFAULT_HP);
	}
	else
	{
		isContainCam = 1;
	}
}

void CZombie::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ZOMBIE_BBOX_WIDTH;
	b = y + ZOMBIE_BBOX_HEIGHT;
}

void CZombie::Death(int _hp)
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
