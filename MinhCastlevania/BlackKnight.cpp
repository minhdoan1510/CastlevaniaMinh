#include "BlackKnight.h"
#include "Brick.h"
#include "Simon.h"

CBlackKnight::CBlackKnight(float _x, float _y)
{
	ani = CAnimations::GetInstance()->Get(BLACKKNIGHT);
	x = _x;
	y = _y;
	nx = 1;
	x_backup = _x;
	distance = GetRandomInt(BLACKKNIGHT_DISTANCE_MIN,BLACKKNIGHT_DISTANCE_MAX);
	enemyType = BLACKKNIGHT;
	vx = BLACKKNIGHT_SPEED_WALKING;
	HP = BLACKKNIGHT_DEFAULT_HP;
	DamageofEnemy = BLACKKNIGHT_DAMAGE;
	isActive = 1;
	isArmor = 1;
	isAttack = 0;
}

CBlackKnight::~CBlackKnight()
{

}

void CBlackKnight::Render()
{
	RenderBoundingBox();
	ani->Render(x, y, nx == 1);
	CEnemy::Render();
}

void CBlackKnight::ChangeDirect()
{
	nx = -nx;
	x_backup = x;
	distance = GetRandomInt(BLACKKNIGHT_DISTANCE_MIN, BLACKKNIGHT_DISTANCE_MAX);
	float _xSimon, _ySimon;
	CSimon::GetIntance()->GetPosition(_xSimon, _ySimon);
	if(isAttack)
	{
		if (nx == 1)
			distance += abs((x + distance / 2) - _xSimon);
		else
			distance += abs((x + distance / 2) - _xSimon);
	}
}

void CBlackKnight::Update(DWORD dt, vector<LPGAMEOBJECT>* _objects)
{
	if (IsDead)
		return;
	vx = nx * BLACKKNIGHT_SPEED_WALKING;
	CGameObject::Update(dt);
	float _xSimon, _ySimon;
	CSimon::GetIntance()->GetPosition(_xSimon, _ySimon);

	if (abs(_ySimon - y) <= BLACKKNIGHT_Y_ATTACK_SIMON)
	{
		if (nx == -1&& isAttack ==0 )
			ChangeDirect();
		isAttack = 1;
	}
	else
		isAttack = 0;
	vy += BLACKKNIGHT_GRAVITY * dt;
	if (abs(x_backup - x) > distance)
		ChangeDirect();

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
		{
			vx = 0;
			ChangeDirect();
		}
		if (ny != 0)
		{
			vy = 0;
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (e->ny != 0)
				{
					RECT rect = static_cast<CBrick*>(e->obj)->GetBBox();
					if (x + BLACKKNIGHT_BBOX_WIDTH > rect.right)
					{
						ChangeDirect();
						x += rect.right - (x + BLACKKNIGHT_BBOX_WIDTH)- nx * 0.4f;
					}
					else if (x < rect.left)
					{
						ChangeDirect();
						x += rect.left -x + nx * 0.4f;
					}
					break;
				}
			}
		}

		/*if (nx != 0)
			nx = -nx;*/
	}
}

void CBlackKnight::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLACKKNIGHT_BBOX_WIDTH;
	b = y + BLACKKNIGHT_BBOX_HEIGHT;
}

void CBlackKnight::Death(int _hp)
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
