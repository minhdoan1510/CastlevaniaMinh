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
	isOnGround = 0;
	lifetime = 0;
	y -= ITEM_PULL_Y;//kéo item lên để tránh overlap
}

CItem::~CItem()
{
}

void CItem::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = this->x;
	t = this->y;
	D3DXVECTOR2 framesize;
	switch (ItemType)
	{
	case HEART_ITEM:
		framesize = HEART_ITEM_SIZE;
		break;
	case HEART_BIG_ITEM:
		framesize = HEART_BIG_ITEM_SIZE;
		break;
	case WHIP_ITEM:
		framesize = WHIP_ITEM_SIZE;
		break;
	case KNIFE_ITEM:
		framesize = KNIFE_ITEM_SIZE;
		break;
	case AXE_ITEM:
		framesize = AXE_ITEM_SIZE;
		break;
	case BOOMERANG_ITEM:
		framesize = BOOMERANG_ITEM_SIZE;
		break;
	case MONEY_BLU_ITEM:case MONEY_RED_ITEM:case MONEY_WHITE_ITEM:
		framesize = MONEY_ITEM_SIZE;
		break;
	case GUNPOWDER_ITEM:
		framesize = GUNPOWDER_ITEM_SIZE;
		break;
	case DOUBLE_WEAPON:
		framesize = DOUBLE_WEAPON_SIZE;
	case CROWM_YELLOW_ITEM:
		framesize = CROWM_YELLOW_SIZE;
		break;
	case CHICKEN_ITEM:
		framesize = CHICKEN_SIZE;
		break;
	}

	r = l + framesize.x;
	b = t + framesize.y;
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
	if (lifetime == 0)
	{
		lifetime = GetTickCount64();
	}
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