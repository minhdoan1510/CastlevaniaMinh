#include "WhiteSkeletonWeapon.h"
#include "Item.h"
#include "Simon.h"

CWhiteSkeletonWeapon::CWhiteSkeletonWeapon(float _x, float _y, int _nx)
{
	ani = CAnimations::GetInstance()->Get(WHITESKELETON_WEAPON);
	objType = weaponType = WHITESKELETON_WEAPON;
	isFinish = false;
	x = _x;
	y = _y;
	nx = _nx;
	vy = -WHITESKELETONWEAPON_VFLY;
	isStart = 0;
}

CWhiteSkeletonWeapon::~CWhiteSkeletonWeapon()
{
}

void CWhiteSkeletonWeapon::Render()
{
	if (!isStart) return;
	if (isFinish) return;
	RenderBoundingBox();
	ani->Render(x, y, nx < 0);
}

void CWhiteSkeletonWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isStart) return;
	if (nx > 0)
		vx = WHITESKELETONWEAPON_SPEED;
	else
		vx = -WHITESKELETONWEAPON_SPEED;

	CGameObject::Update(dt);
	vy += WHITESKELETONWEAPON_GRAVITY * dt;

	if (isFinish) { vx = 0; vy = 0; return; }

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<CGameObject*>* colisions_object= new vector<CGameObject*>();
	colisions_object->push_back(CSimon::GetIntance());

	CalcPotentialCollisions(colisions_object, coEvents); //chỉ xét va chạm đối với simon
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

		for (size_t i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			static_cast<CSimon*>(e->obj)->SetInjured(WHITESKELETONWEAPON_DAMAGE,-nx);
			break;
		}

		isFinish = true;

		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	if (!IsContain(GetBBox(), CCamera::GetInstance()->GetRectCam()))
	{
		isFinish = 1;
	}
}

bool CWhiteSkeletonWeapon::Attack(float _x, float _y, int _nx)
{
	
	return true;
}

void CWhiteSkeletonWeapon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = this->x;
	t = this->y;
	r = x + WHITESKELETONWEAPON_BBOX_WIDTH;
	b = y + WHITESKELETONWEAPON_BBOX_HEIGHT;
}

int CWhiteSkeletonWeapon::GetDamage()
{
	return WHITESKELETONWEAPON_DAMAGE;
}

bool CWhiteSkeletonWeapon::IsStart()
{
	return isStart;
}

void CWhiteSkeletonWeapon::Start(float _x, float _y, int _nx)
{
	isStart = 1;
	x = _x;
	y = _y;
}
