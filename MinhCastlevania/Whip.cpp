#include "Whip.h"
#include"Item.h"


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
		break;
	case 2:
		weaponType = WHIP_LV2;
		whipWidth = WHIP_2_WIDTH;
		break;
	case 3:
		weaponType = WHIP_LV3;
		whipWidth = WHIP_3_WIDTH;
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
		isFinish = false;
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
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->GetObjType() != SIMON && IsContain(GetBBox(), coObjects->at(i)->GetBBox()))
			{
				if (!dynamic_cast<CItem*>(coObjects->at(i)))
					coObjects->at(i)->Death();
			}
		}
	}
}

void CWhip::Render()
{
	if (isFinish) return;

	if (ani->GetCurrentFrame() == 2)
	{

		RenderBoundingBox();
		if (nx > 0)
		{
			ani->Render(x - 24, y, 1);
		}
		else
		{
			ani->Render(x - 75, y, 0);
		}
	}
	else if (ani->GetCurrentFrame() == 1)
	{
		if (nx > 0)
		{
			ani->Render(x - 25, y, 1);
		}
		else
		{
			ani->Render(x - 75, y, 0);
		}
	}
	else
	{
		if (nx > 0)
		{
			ani->Render(x - 23, y, 1);
		}
		else
		{
			ani->Render(x - 80, y, 0);
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
		l = x + 60; // trừ BBOX của Simon
	}
	else
	{
		l = x - whipWidth;
	}
	r = l + whipWidth;
}