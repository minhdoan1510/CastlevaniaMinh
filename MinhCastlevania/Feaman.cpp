#include "Feaman.h"
#include "Simon.h"
#include "Brick.h"
#include "Whip.h"

CFeaman::CFeaman(float _x, float _y)
{
	x = _x;
	y = _y;
	enemyType = FEAMAN;
	ani = CAnimations::GetInstance()->Get(FEAMAN);
	isActive = 0;
	HP = FEAMAN_DEFAULT_HP;
	DamageofEnemy = FEAMAN_DAMAGE;
	isArmor = 1;
	isPrepare = 0;
	timeFollow = 0;
	amountJump = 0;
}

CFeaman::~CFeaman()
{
}

void CFeaman::Jump(int direct, int isJumpHight)
{
	if (isJumping) return;
	//if (timeJump == 0 || GetTickCount() - timeJump <= FEAMAN_TIME_BETWEEN_JUMP) return;

	//timeJump = 0;
	if (direct == 0)
	{
		if (GetRandomInt(0, 1))
			direct = 1;
		else
			direct = -1;
	}
	if (isJumpHight == -1) // nhảy random hight or short
	{
		//if (GetRandomInt(0, 4) == 0)
		//	SetJumpH(direct);
		//else
		SetJumpS(direct);
	}
	else // nhảy theo isJumpHight
	{
		if (isJumpHight == 0)
			SetJumpS(direct);
		else
			SetJumpH(direct);
	}
	nx = direct;
	isJumping = 1;
}

void CFeaman::SetJumpH(int direct)
{
	vy = -FEAMAN_JUMP_HIGHT;
	vx = direct * FEAMAN_SPEED;
}

void CFeaman::SetJumpS(int direct)
{
	vy = -FEAMAN_JUMP_SHORT;
	vx = direct * FEAMAN_SPEED * 2;
}

void CFeaman::Render()
{
	if (isActive||isPrepare)
	{
		RenderBoundingBox();
		ani->Render(x, y, nx > 0);
		CEnemy::Render();
	}
}

void CFeaman::Update(DWORD dt, vector<LPGAMEOBJECT>* _objects)
{
	if (!isFollow && GetTickCount() - timeFollow >= FEAMAN_TIME_FOLLOW_SIMON) {
		isFollow = 1;
	}

	if (GetTickCount() - timePrepare >= FEAMAN_TIME_PREPARE && isPrepare) {
		isActive = 1;
	}
	if (isActive)
	if (!isJumping)
	{
		if (CSimon::GetIntance()->IsAttack())
		{
			RECT rectSimon = CSimon::GetIntance()->GetBBox();
			RECT rectFeaman = GetBBox();
			if (CSimon::GetIntance()->GetNx() == 1 && rectSimon.left < rectFeaman.left)
			{
				if (CSimon::GetIntance()->IsAttackMainWeapon())
				{
					if (abs(rectSimon.right - rectFeaman.left) <= WHIP_3_WIDTH)
					{
						Jump(-1, 1);
						DebugOut(L"Nhay trai");
					}
				}
				else
				{
					unordered_map<int, CWeapon*> weaponList = CSimon::GetIntance()->GetListWeapon();
					for (pair<int, CWeapon*> item : weaponList)
					{
						if (item.first == WHIP) continue;
						if (item.second->IsFinish()) continue;
						LPCOLLISIONEVENT e = SweatAABBx_SafeEnemy(item.second, FEAMAN_DISTANCE_SAFE_WITH_2ND_WEAPON);

						if (e->t > 0 && e->t <= 1.0f)
						{
							Jump(-1, 1);
							DebugOut(L"Nhay trai");
							break;
						}
					}
				}
			}
			if (CSimon::GetIntance()->GetNx() == -1 && rectSimon.left > rectFeaman.left)
			{
				if (CSimon::GetIntance()->IsAttackMainWeapon())
				{
					if (abs(rectSimon.left - rectFeaman.right) <= WHIP_3_WIDTH)
					{
						Jump(1, 1);
						DebugOut(L"Nhay phai");
						//Dap datDap datNhay phaiDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap datDap dat
					}
				}
				else
				{
					unordered_map<int, CWeapon*> weaponList = CSimon::GetIntance()->GetListWeapon();
					for (pair<int, CWeapon*> item : weaponList)
					{
						if (item.first == WHIP) continue;
						if (item.second->IsFinish()) continue;
						float _vx, _vy;
						LPCOLLISIONEVENT e = SweatAABBx_SafeEnemy(item.second, FEAMAN_DISTANCE_SAFE_WITH_2ND_WEAPON);

						if (e->t > 0 && e->t <= 1.0f)
						{
							Jump(1, 1);
							DebugOut(L"Nhay phai");
							break;
						}
					}
				}
			}
		}
	}
	if (isActive)
	{
		if (isFollow)
		{
			if (!isJumping)
			{
				float xSimon, ySimon;
				CSimon::GetIntance()->GetPosition(xSimon, ySimon);

				if ((xSimon - x) > 0)
					nx = 1;
				else
					nx = -1;
				if (abs(xSimon - x) <= FEAMAN_DISTANCE_TRICK_JUMP_S)
					Jump(nx, 0);
				else if (abs(xSimon - x) <= FEAMAN_DISTANCE_TRICK_JUMP_H)
					Jump(nx, 1);
				else
					Jump(nx);
			}
			if (IsContain(GetBBox(), CSimon::GetIntance()->GetBBox()))
			{
				timeFollow = GetTickCount();
				isFollow = 0;
			}
		}
		else
		{
			Jump();
		}

		if (amountJump >1 &&!IsContain(GetBBox(), CCamera::GetInstance()->GetRectCam()))
		{
			IsDead = 1;
		}
	}
	else
	{
		if (!isPrepare && IsContain(GetBBox(), CCamera::GetInstance()->GetRectCam()))
		{
			isPrepare = 1;
			timePrepare = GetTickCount();
		}
	}
	
	vy += FEAMAN_GRAVITY * dt;
	CGameObject::Update(dt);
	//DebugOut(L"%.6f \n", vy);
	//Collision with Brick
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
		if (ny < 0)
		{
			y += min_ty * dy + ny * 0.4f;
			/*if (timeJump == 0)
			{
				DebugOut(L"Dap dat");
				timeJump = GetTickCount();
			}*/
			isJumping = 0;
		}
		else
		{
			y += dy;
		}
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CFeaman::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FEAMAN_BBOX_WIDTH;
	b = y + FEAMAN_BBOX_HEIGHT;
}

void CFeaman::Death(int _hp)
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