#include "WhiteSkeleton.h"
#include "Simon.h"
#include "Brick.h"

CWhiteSkeleton::CWhiteSkeleton(float _x, float _y)
{
	x = x_backup = _x;
	y = _y;
	enemyType = WHITESKELETON;
	ani = CAnimations::GetInstance()->Get(WHITESKELETON);
	isActive = 0;
	DamageofEnemy = WHITESKELETON_DAMAGE;
	HP = WHITESKELETON_DEFAULT_HP;
	isArmor = 1;
	nx = 1;
	distance = WHITESKELETON_DISTANCE_MOVE; 
	lTimeAttack = 0;
	lTimeMultiAttack = 0;
}

CWhiteSkeleton::~CWhiteSkeleton()
{
}

void CWhiteSkeleton::Render()
{
	if (!isActive)
		return;
	RenderBoundingBox();
	//if (weapon->IsFinish())
	//	weapon->Render();

	ani->Render(x, y, x_Simon > x);
	if (weapon.size() > 0)
		for (int i = 0; i < weapon.size(); i++)
		{
			weapon.at(i)->Render();
		}
}

void CWhiteSkeleton::Jump(int direct)
{
	nx = direct;
	vy = -WHITESKELETON_VJUMP*2;
	vx = direct * WHITESKELETON_SPEED_WALKING;
	isJumping = 1;
}

void CWhiteSkeleton::ChangeDirect()
{
	if (!isJumping)
	{
		x_backup = x;
		nx = -nx;
	}
}

CBrick* CWhiteSkeleton::FilterBrickCanJump(D3DXVECTOR2 _point, int _nx, vector<CGameObject*>* _bricks)
{
	float mintx = 99999;
	float minty = 99999;
	float y_result = 99999;
	float minDis = 99999;
	CGameObject* result = NULL;
	if (_nx > 0)
	{
		for (int i = 0; i < _bricks->size(); i++)
		{
			RECT rect = _bricks->at(i)->GetBBox();
			if ((_point.x <= rect.left  && _point.y - rect.top <= WHITESKELETON_HEIGHT_MAX_JUMP && abs(_point.x - rect.left) <= WHITESKELETON_WIDTH_MAX_JUMP)
				|| (_point.x > rect.left && rect.right > _point.x + WHITESKELETON_WIDTH_MAX_JUMP && _point.y - rect.top <= WHITESKELETON_HEIGHT_MAX_JUMP))
			{
				float dis;
				if (_point.x > rect.left)
					dis = sqrt(pow(WHITESKELETON_WIDTH_MAX_JUMP, 2) + pow(rect.top - _point.y, 2));
				else
					dis = sqrt(pow(_point.x - rect.left, 2) + pow(rect.top - _point.y, 2));

				if (minDis > dis && (rect.right - (_point.x + WHITESKELETON_HEIGHT_MAX_JUMP) > 0))
				{
					if (rect.top > y_result) continue;
					y_result = rect.top;
					mintx = abs(_point.x - rect.left);
					minty = abs(rect.top - _point.y);
					minDis = sqrt(pow(_point.x - rect.left, 2) + pow(rect.top - _point.y, 2));
					result = _bricks->at(i);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < _bricks->size(); i++)
		{
			RECT rect = _bricks->at(i)->GetBBox();
			if ((_point.x >= rect.right && _point.y - rect.top <= WHITESKELETON_HEIGHT_MAX_JUMP && abs(_point.x - rect.right) <= WHITESKELETON_WIDTH_MAX_JUMP)
				|| (_point.x < rect.right && rect.left < _point.x - WHITESKELETON_WIDTH_MAX_JUMP && _point.y - rect.top <= WHITESKELETON_HEIGHT_MAX_JUMP))
			
			{
				float dis;
				if (_point.x > rect.left)
					dis = sqrt(pow(WHITESKELETON_WIDTH_MAX_JUMP, 2) + pow(rect.top - _point.y, 2));
				else
					dis = sqrt(pow(_point.x - rect.right, 2) + pow(rect.top - _point.y, 2));

				if (minDis > dis && (rect.left - (_point.x - WHITESKELETON_HEIGHT_MAX_JUMP) < 0))
				{
					if (rect.top > y_result) continue;
					y_result = rect.top;
					mintx = abs(_point.x - rect.right);
					minty = abs(rect.top - _point.y);
					minDis = sqrt(pow(_point.x - rect.right, 2) + pow(rect.top - _point.y, 2));
					result = _bricks->at(i);
				}
			}
		}
	}
	if (result != NULL)
	{
		RECT r = result->GetBBox();
		DebugOut(L"Searched Brick: %d, %d, %d, %d\r", r.left, r.top, r.right, r.bottom);
	}
	return static_cast<CBrick*>(result);
}

void CWhiteSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	if (IsDead)
		return;
	if (!isActive && IsContain(GetBBox(), CCamera::GetInstance()->GetRectCam()))
	{
		isActive = 1;
	}
	if (!isActive)
		return;
	CGameObject::Update(dt);

	float _xSimon, _ySimon;
	CSimon::GetIntance()->GetPosition(_xSimon, _ySimon);
	x_Simon = _xSimon;
	//Update Weapon
	if (!isAttack&&abs(x - _xSimon) <= WHITESKELETON_DISTANCE_ATTACK && (lTimeAttack == 0 || GetTickCount() - lTimeAttack < WHITESKELETON_SPEED_ATTACK))
	{
		int nXuong = GetRandomInt(1, 3);
		lTimeAttack = GetTickCount();
		for (int i = 0; i < nXuong; i++)
		{
			Attack();
		}
	}

	if (weapon.size() == 0)
	{
		isAttack = 0;
		lTimeAttack = 0;
	}
	else
		for (int i = 0; i < weapon.size(); i++)
		{
			if (!static_cast<CWhiteSkeletonWeapon*>(weapon.at(i))->IsStart())
			{
				if (lTimeMultiAttack == 0 || GetTickCount() - lTimeMultiAttack >= WHITESKELETON_SPEED_MULTI_ATTACK)
				{			
					static_cast<CWhiteSkeletonWeapon*>(weapon.at(i))->Start(x, y, (x_Simon > x) ? 1 : -1);
					if (i == weapon.size() - 1)
						lTimeMultiAttack = 0;
					else
						lTimeMultiAttack = GetTickCount();
				}
		
				break;
			}
			weapon.at(i)->Update(dt, objects);
			if (weapon.at(i)->IsFinish())
				weapon.erase(weapon.begin() + i);
		}

	//Update Enemy
	bool flagDoneGoDistance = 0;
	if (abs(_xSimon - x) < WHITESKELETON_DISTANCE_WITH_SIMON)
	{
		if ((nx == 1 && _xSimon > x) || (nx == -1 && _xSimon < x))
		{
			ChangeDirect();
			flagDoneGoDistance = 1;
			if (_xSimon > x)
				x_backupDistanceWithSimon = _xSimon - WHITESKELETON_DISTANCE_WITH_SIMON;
			else
				x_backupDistanceWithSimon = _xSimon + WHITESKELETON_DISTANCE_WITH_SIMON;
		}

	}
	if (!isJumping&& flagDoneGoDistance)
	{
		if (x_SimonBackup < _xSimon)
		{
			x_SimonBackup = _xSimon;
			nx = (_xSimon > x) ? 1 : -1;
		}
		else if (x_SimonBackup > _xSimon)
		{
			x_SimonBackup = _xSimon;
			nx = (_xSimon > x) ? -1 : 1;
		}
	}

	if (abs(x_backup - x) > distance)// chỉnh distanceMove để follow simon
	{
		if ((nx == 1 && _xSimon > x) || (nx == -1 && _xSimon < x))
		{
			distance = WHITESKELETON_DISTANCE_MOVE + abs(abs(_xSimon - x) - WHITESKELETON_DISTANCE_WITH_SIMON);
		}
		else
		{
			ChangeDirect();
		}
	}

	vx = nx * WHITESKELETON_SPEED_WALKING;
	vy += WHITESKELETON_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<CGameObject*>* colliable_objects = new vector<CGameObject*>(); // filter list Brick

	for (int i = 0; i < objects->size(); i++)
		if (dynamic_cast<CBrick*>(objects->at(i)))
		{
			colliable_objects->push_back(objects->at(i));
		}

	CalcPotentialCollisions(colliable_objects, coEvents);
	if (coEvents.size() == 0)
	{
		if (isJumping)
			DebugOut(L"%.6f  %.6f\n", dx, dy);
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, _nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, _nx, ny);

		x += min_tx * dx + _nx * 0.4f;

		if (ny == -1)
		{
			y += min_ty * dy + ny * 0.4f;
			isJumping = 0;
		}
		else
			y += dy;

		if (_nx != 0)
		{
			vx = 0;
			nx = -nx;
		}
		if (ny != 0)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (e->ny != 0)
				{
					RECT rect = static_cast<CBrick*>(e->obj)->GetBBox();
					if (x + WHITESKELETON_BBOX_WIDTH > rect.right && nx == 1)
					{
						CBrick* brickCanJump = FilterBrickCanJump(D3DXVECTOR2(rect.right, rect.top), 1, colliable_objects);

						if (brickCanJump == NULL)
							ChangeDirect();
						else
							Jump(1);
					}
					else if (x < rect.left&&nx == -1)
					{
						CBrick* brickCanJump = FilterBrickCanJump(D3DXVECTOR2(rect.left, rect.top), -1, colliable_objects);
						if (brickCanJump == NULL)
							ChangeDirect();
						else
							Jump(-1);
					}
					break;
				}
			}
		}
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CWhiteSkeleton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WHITESKELETON_BBOX_WIDTH;
	b = y + WHITESKELETON_BBOX_HEIGHT;
}

void CWhiteSkeleton::Death(int _hp)
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

void CWhiteSkeleton::Attack()
{
	if (isJumping) return;
	weapon.push_back(new CWhiteSkeletonWeapon(x, y, (x_Simon > x) ? 1 : -1));
	isAttack = 1;
}

int CWhiteSkeleton::GetDamage()
{
	return WHITESKELETON_DAMAGE;
}
