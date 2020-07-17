#include "PhantomBat.h"
#include "Simon.h"
#include "Weapon.h"
#include "PhantomBatBullet.h"

float CPhantomBat::CalcSpeed(float _vx)
{
	if (distanceFlew < PHANTOMBAT_FLY_HALF)
		return PHANTOMBAT_FLY_VY_PARABOL(PHANTOMBAT_FORCE_VY, distanceFlew);//float(PHANTOMBAT_FORCE_VY / 65536) * pow(distanceFlew, 2) - (PHANTOMBAT_FORCE_VY / 128) * distanceFlew + PHANTOMBAT_FORCE_VY;
	else
		return -PHANTOMBAT_FLY_VY_PARABOL(PHANTOMBAT_FORCE_VY, distanceFlew);// -(float(PHANTOMBAT_FORCE_VY / 65536) * pow(distanceFlew, 2) - (PHANTOMBAT_FORCE_VY / 128) * distanceFlew + PHANTOMBAT_FORCE_VY);
}

void CPhantomBat::MovePhantomBatPal(int direct, bool isApproach)
{
	vx = PHANTOMBAT_FLY_SLOW * 5 * direct;
	directFly = direct;
	yBackupMovePhantomBat = y;
	vy = (isApproach) ? PHANTOMBAT_FLY_SLOW : PHANTOMBAT_FLY_SLOW * 2;
	xBackupMovePhantomBat = x;
	distanceFlew = 0;
	stateFly = PHANTOMBAT_STATE_FLY_1;
}

void CPhantomBat::UpdateMovePhantomBat(RECT boxSimon)
{
	RECT rectCam = CCamera::GetInstance()->GetRectCam();
	distanceFlew += abs(xBackupMovePhantomBat - x);
	xBackupMovePhantomBat = x;

	if (rectCam.left > x || rectCam.right < GetBBox().right)
		directFly = -1 * directFly;
	switch (stateFly)
	{
	case PHANTOMBAT_STATE_FLY_1:
		vx = PHANTOMBAT_FLY_FAST*directFly;
		vy = CalcSpeed(distanceFlew);
		//DebugOut(L"%4.2f , %4.2f\n", distanceFlew, vy);
		if (boxSimon.top - y >= PHANTOMBAT_DISTANCE_FLY_MAX_WITH_SIMON)
			stateFly = PHANTOMBAT_STATE_FLY_2;
		break;
	case PHANTOMBAT_STATE_FLY_2:
		vx = PHANTOMBAT_FLY_SLOW * directFly;
		vy = PHANTOMBAT_FLY_SLOW;
		if (boxSimon.top - y <= PHANTOMBAT_DISTANCE_FLY_ATTACK_WITH_SIMON)
		{
			stateFly = PHANTOMBAT_STATE_FLY_1;
			statePhantomBat = PHANTOMBAT_STATE_IDLE;
			isProcessing = 0;
			timeIdle = GetTickCount();
		}
		break;
	}

	//DebugOut(L"%4.2f,%4.2f\n", vx, vy);

}

CPhantomBat::CPhantomBat(float _x, float _y)
{
	x = _x;
	y = _y;
	ani = CAnimations::GetInstance()->Get(PHANTOMBAT);
	sprite = CSprites::GetInstance()->Get(PHANTOMBAT_SLEEP);
	vx = 0;
	vy = 0;
	enemyType = PHANTOMBAT;
	statePhantomBat = PHANTOMBAT_STATE_IDLE;
	isFollow = 0;
	isActive = 0;
	timeFollow = 0;
	timeIdle = 0;
	isBoss = 1;
	isArmor = 1;
	HP = PHANTOMBAT_DEFAULT_HP;
	DamageofEnemy = PHANTOMBAT_DAMAGE;
}

CPhantomBat::~CPhantomBat()
{
	
}

void CPhantomBat::Render()
{
	RenderBoundingBox();
	if (isActive)
	{
		ani->Render(x, y, 0);
	}
	else
	{
		sprite->Draw(x, y);
	}

	if (bullet != nullptr && !bullet->IsFinish())
	{
		bullet->Render();
	}
	CEnemy::Render();
}

void CPhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	//DebugOut(L"%d, %d\n", statePhantomBat,(int)timeIdle );

	CGameObject::Update(dt);

	DWORD now = GetTickCount();

	RECT boxSimon = CSimon::GetIntance()->GetBBox();
	RECT boxThis = GetBBox();
	if (!isActive && abs(boxSimon.left - x) <= PHANTOMBAT_DISTANCE_ACTIVE)
	{
		isActive = true;
		timeIdle = now;
	}
	else if(!isActive)
		return;


	switch (statePhantomBat)
	{
	case PHANTOMBAT_STATE_IDLE:
		vx = vy = 0;
		if (now - timeIdle >= PHANTOMBAT_TIME_IDLE)
		{

			timeIdle = now;
			if (boxSimon.top - boxThis.top < 0)
			{
				statePhantomBat = PHANTOMBAT_STATE_ATTACK_WEAPON;
				if (bullet == nullptr || bullet->IsFinish())
				{
					D3DXVECTOR2 nDirect(boxSimon.left - boxThis.left, boxSimon.top - boxThis.top);
					SAFE_DELETE(bullet);
					bullet = new CPhantomBatBullet(x, y);
					bullet->SetVectorMove(nDirect.x, nDirect.y);
				}
			}
			else
			if (boxSimon.top - boxThis.top >= PHANTOMBAT_DISTANCE_FLY_MAX_WITH_SIMON)
			{
				statePhantomBat = PHANTOMBAT_STATE_APPROACH_DOWN;
				//DebugOut(L"Test");
			}
			else if (abs(boxThis.left - boxSimon.left) >= PHANTOMBAT_DISTANCE_NEED_APPROACH)
			{
				statePhantomBat = PHANTOMBAT_STATE_APPROACH;
			}
			else
			{
					statePhantomBat = PHANTOMBAT_STATE_ATTACK;
			}
		}
		break;
	case PHANTOMBAT_STATE_APPROACH_DOWN:
		vx = 0;
		vy = PHANTOMBAT_FLY_SLOW;
		if (boxSimon.top - boxThis.top < PHANTOMBAT_DISTANCE_FLY_MAX_WITH_SIMON)
		{
			statePhantomBat = PHANTOMBAT_STATE_IDLE;
			timeIdle = now;
		}
		break;
	case PHANTOMBAT_STATE_APPROACH:
		if (!isProcessing)
		{
			MovePhantomBatPal((boxSimon.left < boxThis.left) ? -1 : 1, 1);
			isProcessing = 1;
		}
		break;
	case PHANTOMBAT_STATE_ATTACK:
		if (!isProcessing)
		{
			MovePhantomBatPal((boxSimon.left < boxThis.left) ? -1 : 1, 1);
			isProcessing = 1;
		}
		break;
	case PHANTOMBAT_STATE_ATTACK_WEAPON:
		vy = -PHANTOMBAT_FLY_SLOW;
		vx = 0;
		if (boxSimon.top - y >= PHANTOMBAT_DISTANCE_FLY_ATTACK_WITH_SIMON)
		{
			statePhantomBat = PHANTOMBAT_STATE_IDLE;
			timeIdle = now;
		}

		break;

	default:
		break;
	}

	if (isProcessing)
		UpdateMovePhantomBat(boxSimon);

	x += vx * dt;
	y += vy * dt;


	//UpdateBullet
	if (bullet != nullptr && !bullet->IsFinish())
	{
		bullet->Update(dt);
	}
}

void CPhantomBat::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	t = y;
	b = y + PHANTOMBAT_BBOX_1_HEIGHT;
	l = x;
	r = x + PHANTOMBAT_BBOX_2_WIDTH;
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

ObjectType CPhantomBat::GetItemHolder()
{
	return END_ITEM;
}