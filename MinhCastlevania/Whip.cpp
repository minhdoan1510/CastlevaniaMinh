#include "Whip.h"
#include "Item.h"
#include "Simon.h"
#include "Enemy.h"
#include "Sound.h"
#include "Brick.h"




CWhip::CWhip() :CWeapon::CWeapon()
{
	SetLever(1);
	objType = WHIP;
	//lifeTime = 0;
}

CWhip::~CWhip()
{
}

void CWhip::SetLever(int lv)
{
	switch (lv)
	{
	case 1:
		weaponType = WHIP_LV1;
		whipWidth = WHIP_1_WIDTH;
		damage = WHIP_1_DAMAGE;
		break;
	case 2:
		weaponType = WHIP_LV2;
		whipWidth = WHIP_2_WIDTH;
		damage = WHIP_2_DAMAGE;
		break;
	case 3:
		weaponType = WHIP_LV3;
		whipWidth = WHIP_3_WIDTH;
		damage = WHIP_3_DAMAGE;
		break;
	}
	//ani = CAnimationSets::GetInstance()->Get(WHIP)->at(weaponType);
	ani = CAnimations::GetInstance()->Get(weaponType);
	leverWhip = lv;
}

bool CWhip::Attack(float _x, float _y, int _nx)
{
	if (lifeTime == 0 || GetTickCount64() - lifeTime > WHIP_SPEED_ATTACK)
	{
		lifeTime = GetTickCount64();
		x = _x;
		y = _y;
		nx = _nx;
		isFinish = 0;
		isDamage = 0;
		ani->ResetFarmeCurrent();
		return true;
	}
	return false;
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFinish) return;

	if (ani->GetCurrentFrame() == 2)
	{
		if (!isDamage)
		{
			for (int i = 0; i < coObjects->size(); i++)
			{
				if (coObjects->at(i)->GetObjType() != SIMON && IsContain(GetBBox(), coObjects->at(i)->GetBBox()))
				{
					if (dynamic_cast<CItem*>(coObjects->at(i))) continue;
					if (!dynamic_cast<CBrick*>(coObjects->at(i)) || static_cast<CBrick*>(coObjects->at(i))->IsDetroy() == 1)
						CSound::GetInstance()->play("Hit", 0, 1);
					if (dynamic_cast<CEnemy*>(coObjects->at(i)))
					{
						isDamage = 1;
						static_cast<CEnemy*>(coObjects->at(i))->Death(GetDamage());
					}
					else
						coObjects->at(i)->Death();
				}
			}
		}
	}
}

void CWhip::Render()
{
	if (isFinish) return;

	if (CSimon::GetIntance()->IsFreeze() && CSimon::GetIntance()->IsEndgameState())
	{
		if (ani->GetCurrentFrame() == 2)
		{
			RenderBoundingBox();
			if (nx > 0)
			{
				ani->RenderFrame(2,x - X_FRAME_2_RIGHT, y, 1);
			}
			else
			{
				ani->RenderFrame(2, x - X_FRAME_2_LEFT, y, 0);
			}
		}
		else if (ani->GetCurrentFrame() == 1)
		{
			if (nx > 0)
			{
				ani->RenderFrame(1,x - X_FRAME_1_RIGHT, y, 1);
			}
			else
			{
				ani->RenderFrame(1,x - X_FRAME_1_LEFT, y, 0);
			}
		}
		else
		{
			if (nx > 0)
			{
				ani->RenderFrame(0, x - X_FRAME_0_RIGHT, y, 1);
			}
			else
			{
				ani->RenderFrame(0, x - X_FRAME_0_LEFT, y, 0);
			}
		}
		return;
	}
	else
	{
		if (ani->GetCurrentFrame() == 2)
		{

			RenderBoundingBox();
			if (nx > 0)
			{
				ani->Render(x - X_FRAME_2_RIGHT, y, 1);
			}
			else
			{
				ani->Render(x - X_FRAME_2_LEFT, y, 0);
			}
		}
		else if (ani->GetCurrentFrame() == 1)
		{
			if (nx > 0)
			{
				ani->Render(x - X_FRAME_1_RIGHT, y, 1);
			}
			else
			{
				ani->Render(x - X_FRAME_1_LEFT, y, 0);
			}
		}
		else
		{
			if (nx > 0)
			{
				ani->Render(x - X_FRAME_0_RIGHT, y, 1);
			}
			else
			{
				ani->Render(x - X_FRAME_0_LEFT, y, 0);
			}
		}
	}

	if (ani->GetDoneFrameFinal())
		isFinish = 1;
}

void CWhip::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	t = y + WHIP_BBOX_Y;
	b = t + WHIP_HEIGHT;
	if (nx > 0)
	{
		l = x + SIMON_BBOX_WIDTH/2+ SIMON_BBOX_WIDTH / 2.5; // trừ BBOX của Simon
	}
	else
	{
		l = x - whipWidth;
	}
	r = l + whipWidth;
}

int CWhip::GetDamage()
{
	return damage;
}
