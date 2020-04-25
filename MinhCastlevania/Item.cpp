#include"Item.h"

CItem::CItem(float _x, float _y, ObjectType _type)
{
	objType = ITEM;
	ItemType = _type;
	sprite = CSprites::GetInstance()->Get(ItemType);
	IsDead = false;
	x_backup = _x;
	x = _x;
	y = _y;
	if (_type == HEART_ITEM)
		vx = ITEM_SPEED_FLY;
	else
		vx = 0;
	lifetime = GetTickCount64();
	isOnGround = 0;
}

CItem::~CItem()
{
}

void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = this->x;
	t = this->y;

	switch (ItemType)
	{
	case HEART_ITEM:
		r = x + 16;
		b = y + 16;
		break;
	case HEART_BIG_ITEM:
		r = x + 24;
		b = y + 20;
		break;
	case WHIP_ITEM:
		r = x + 32;
		b = y + 32;
		break;
	case KNIFE_ITEM:
		r = x + 30;
		b = y + 30;
		break;
	case AXE_ITEM:
		r = x + 30;
		b = y + 28;
		break;
	case BOOMERANG_ITEM:
		r = x + 28;
		b = y + 28;
		break;
	case MONEY_BLU_ITEM:case MONEY_RED_ITEM:case MONEY_WHITE_ITEM:
		r = x + 30;
		b = y + 30;
		break;
	case GUNPOWDER_ITEM:
		r = x + 30;
		b = y + 30;
		break;
	case DOUBLE_WEAPON:
		r = x + 28;
		b = y + 28;
		break;
	}
}

void CItem::Render()
{
	if (IsDead) return;
	sprite->Draw(x,y);
	RenderBoundingBox();
}

void CItem::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (IsDead) return;
	if (!isOnGround && ItemType == HEART_ITEM)
	{
		if (x <= x_backup - WIDTH_MOVE_HEART)
			vx = ITEM_SPEED_FLY;
		else if(x >= x_backup + WIDTH_MOVE_HEART )
			vx = -ITEM_SPEED_FLY;
	}
	if (isOnGround)
	{
		vx = 0;
	}

	CGameObject::Update(dt);
	vy = ITEM_GAME_GRAVITY;

	if (GetTickCount64() - lifetime > ITEM_LIFE_TIME)
	{
		this->IsDead = true;
		return;

	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		x += dx;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0)
		{
			vy = 0;
			isOnGround = true;
		}
	}
}

ObjectType CItem::GetItemType()
{
	return ItemType;
}

void CItem::Death()
{
	IsDead = true;
}