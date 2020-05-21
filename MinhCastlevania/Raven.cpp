#include "Raven.h"
#include "Simon.h"
#include "Whip.h"

CRaven::CRaven(float _x, float _y)
{
	x = _x;
	y = _y;
	enemyType = RAVEN;
	sprite = CSprites::GetInstance()->Get(RAVEN_SLEEP);
	ani = CAnimations::GetInstance()->Get(RAVEN);
	isActive = 0;
	HP = RAVEN_DEFAULT_HP;
	DamageofEnemy = RAVEN_DAMAGE;
	isAttackBottomOrTopofWhip = rand() % 2; 
	Attack = 0;
	EndAttack = 0;
	isAutoFly = 0;
	isArmor = 0;
}

void CRaven::FlyPoint(float _x, float _y)
{
	float nxDirect, nyDirect;
	nxDirect = _x - x;
	nyDirect = _y - y;
	vx = ((float)nxDirect/nyDirect)*RAVEN_SPEED_Y_FLY;
	vy = RAVEN_SPEED_Y_FLY;

	if (x - _x> 0)
		nx = -1;
	else
		nx = 1;

	if (_y - y > 0)
		nyCP = -1;
	else
		nyCP = 1;

	XCP = _x;
	YCP = _y;
	isAutoFly = 1;
}

void CRaven::UnFollow()
{
	isFollow = 0;
	timeFollow = GetTickCount();
}

CRaven::~CRaven()
{
}

void CRaven::Render()
{
	RenderBoundingBox();
	if (isActive)
	{
		ani->Render(x, y, nx > 0);
	}
	else 
	{
		sprite->Draw(x, y);
	}
	CEnemy::Render();
}

void CRaven::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	if (!isFollow && GetTickCount() - timeFollow >= RAVEN_TIME_FOLLOW_SIMON) {
		isFollow = 1;
	}
	if (!isAutoFly)
	{
		if (isActive)
		{
			if (isFollow)
			{
				if (EndAttack)
				{
					//nx = -nx;
					vx = RAVEN_SPEED_FLY_ATTACKING * -nx;
					vy = 0;
				}
				else
				{
					float xSimon, ySimon;
					CSimon::GetIntance()->GetPosition(xSimon, ySimon);

					if ((ySimonBackup  == 0)||(!Attack && (abs(ySimonBackup - y) <= 5) && (ySimonBackup != ySimon + WHIP_BBOX_Y + WHIP_HEIGHT + 1 && ySimonBackup != ySimon + WHIP_BBOX_Y - RAVEN_BBOX_HEIGHT - 1)))
					{
						
						if (rand()%2)
							ySimonBackup = ySimon + WHIP_BBOX_Y - RAVEN_BBOX_HEIGHT - 1; // -1 tránh trong phạm vi của Whip
						else
							ySimonBackup = ySimon + WHIP_BBOX_Y + WHIP_HEIGHT + 1; // +1 tránh trong phạm vi của Whip

						if (x - xSimon > 0)
						{
							xSimonBackup = xSimon - RAVEN_DISTANCE_FLY_SIMON;
						}
						else
						{
							xSimonBackup = xSimon + RAVEN_DISTANCE_FLY_SIMON; //nhằm cho reven bay quá simon
						}
						Attack = 0;
						UnFollow();
					}

					if (!Attack && abs(ySimonBackup - y) <= 5)
					{
						y = ySimonBackup; 
						UnFollow();
						vy = 0;
						if (abs(xSimonBackup - x) < RAVEN_DISTANCE_TRICK_SIMON)
						{
							Attack = 1;
						}
						else
						{
							if (xSimonBackup - x < 0)
								FlyPoint((xSimonBackup + x) / 2, y + SIMON_BBOX_HEIGHT);
							else
								FlyPoint((xSimonBackup + x) / 2, y + SIMON_BBOX_HEIGHT);

							return;
						}
					}

					if (abs(xSimonBackup - x) < 5)
					{
						EndAttack = 1;
						UnFollow();
					}

					if ((xSimonBackup - x) > 0)
						nx = 1;
					else
						nx = -1;
					vx = nx * RAVEN_SPEED_X_FLY;

					if (Attack)
					{
						DebugOut(L"Âtkc\n");
						vx = RAVEN_SPEED_FLY_ATTACKING*nx;
						vy = 0;
						//UnFollow();
					}
					else
					{
						if (ySimonBackup < y)
						{
							vy = -RAVEN_SPEED_Y_FLY;
						}
						else if (ySimonBackup > y)
						{
							vy = RAVEN_SPEED_Y_FLY;
						}
					}
				}
			}
			else
			{
				vx = vy = 0;
			}

			CGameObject::Update(dt);
			x += dx;
			y += dy;
		}
		else
		{
			float xSimon, ySimon;
			CSimon::GetIntance()->GetPosition(xSimon, ySimon);
			if (IsContain(this->GetBBox(), CCamera::GetInstance()->GetRectCam()))
			{
				isActive = 1;
				isFollow = 0;
				timeFollow = GetTickCount();
			}
		}
	}
	else
	{
		CGameObject::Update(dt);
		x += dx;
		y += dy;
		if ((XCP - x <= 0 && nx == 1) || (XCP - x >= 0 && nx == -1))// || (YCP - y <= 0 && nyCP == 1) || (nyCP == -1 && YCP - y >= 0))
		{
			DebugOut(L"End\n");
			UnFollow();
			isAutoFly = 0;
		}
	}
}

void CRaven::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + RAVEN_BBOX_WIDTH;
	b = y + RAVEN_BBOX_HEIGHT;
}

void CRaven::Death(int _hp)
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
