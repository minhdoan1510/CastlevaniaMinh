#include "PhantomBat.h"
#include "Simon.h"

float CPhantomBat::CalcSpeed()
{
	return (float)(double)((double)3 / 128) * pow(vx, 2) - (double)3 / 64;
}

void CPhantomBat::MovePhantomBat(int direct,int isApproach)
{
	if (isMovePhantomBat) return;
	vx = PHANTOMBAT_FLY_FAST * direct; 
	yBackupMovePhantomBat = y;
	vy = (isApproach) ? PHANTOMBAT_VY_APPROACH : PHANTOMBAT_FLY_SLOW;
}

void CPhantomBat::UpdateMovePhantomBat(int direct)
{
	D3DXVECTOR2 posSimon;
	CSimon::GetIntance()->GetPosition(posSimon.x, posSimon.y);
	if (y <= posSimon.x - PHANTOMBAT_DISTANCE_FLY_MAX_WITH_SIMON)
	{
		vx = vy = PHANTOMBAT_FLY_SLOW;
	}
}

CPhantomBat::CPhantomBat(float _x, float _y)
{
	x = _x;
	y = _y;
	ani = CAnimations::GetInstance()->Get(PHANTOMBAT);
	sprite = CSprites::GetInstance()->Get(PHANTOMBAT_SLEEP);
	vx = PHANTOMBAT_FLY_SLOW*3;
	vy = PHANTOMBAT_FLY_SLOW;
	enemyType = PHANTOMBAT;
	isFollow = 0;
	isActive = 0;
	timeFollow = 0;
	timeIdle=0;
}

CPhantomBat::~CPhantomBat()
{
}

void CPhantomBat::Render()
{
	if (isActive)
	{
		ani->Render(x, y, 0);
	}
	else
	{
		sprite->Draw(x, y);
	}
}

void CPhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	D3DXVECTOR2 posSimon;
	RECT rectSimon;
	RECT rectCam;
	RECT rectThis;
	CSimon::GetIntance()->GetPosition(posSimon.x, posSimon.y);
	rectSimon = CSimon::GetIntance()->GetBBox();
	rectCam = CCamera::GetInstance()->GetRectCam();
	rectThis = GetBBox();
	DWORD now = GetTickCount();
	if (isActive)
	{
		switch (statePhantomBat)
		{
		case PHANTOMBAT_STATE_IDLE:
			if (now - timeIdle <= PHANTOMBAT_TIME_IDLE)
			{
				if (abs(x - posSimon.x) <= PHANTOMBAT_DISTANCE_NEED_APPROACH)
				{
					statePhantomBat = PHANTOMBAT_STATE_APPROACH;
				}
				else
				{
					if (posSimon.y - y > 0)
						statePhantomBat = PHANTOMBAT_STATE_ATTACK_WEAPON;
					else
						statePhantomBat = PHANTOMBAT_STATE_ATTACK;
				}
			}
			break;
		case PHANTOMBAT_STATE_APPROACH:
			//((x - posSimon.x > 0) ? -1 : 1)
			break;
		case PHANTOMBAT_STATE_ATTACK:
			break;
		case PHANTOMBAT_STATE_ATTACK_WEAPON:
			break;
		}
		
		CGameObject::Update(dt);
		x += dx;
		y += dy;
	}
	else
	{
		if (abs(posSimon.x - x)<=PHANTOMBAT_DISTANCE_ACTIVE)
		{
			isActive = true;
		}
	}
}

void CPhantomBat::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	t = y;
	b = y + PHANTOMBAT_BBOX_1_HEIGHT;
	if (ani->GetCurrentFrame() == 0)
	{
		l = x + (PHANTOMBAT_BBOX_2_WIDTH - PHANTOMBAT_BBOX_1_WIDTH) / 2;
		r = x + PHANTOMBAT_BBOX_1_WIDTH;
	}
	else
	{
		l = x;
		r = x + PHANTOMBAT_BBOX_2_WIDTH;
	}
}

void CPhantomBat::Death(int _hp)
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