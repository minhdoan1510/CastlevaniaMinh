#include "Brick.h"
#include "TextureManager.h"
#include "GameDefine.h"

CBrick::CBrick(float X, float Y, int W, int H, int _type, ObjectType _item)
{
	typeBrick = static_cast<ObjectType>(_type);
	x = X;
	y = Y;
	width = W;
	height = H;
	objType = ObjectType::BRICK;
	if (typeBrick == BRICK_MODEL_TRANSPARENT_1 || typeBrick == BRICK_MODEL_TRANSPARENT_2) return;
	isDestroy = 1;
	if (_item != Null)
		itemHolder = _item;
	sprite = CSprites::GetInstance()->Get(typeBrick);
	if (sprite == NULL)
		DebugOut(L"[Brick] Sprite brick type %d not found in CSprites\n",_type);
}

void CBrick::Render()
{
	RenderBoundingBox();
	if (typeBrick == BRICK_MODEL_TRANSPARENT_1 || typeBrick == BRICK_MODEL_TRANSPARENT_2)
		return;
	sprite->Draw(x, y);
}

int CBrick::GetTypeBrick()
{
	return typeBrick;
}

void CBrick::Death()
{
	if (typeBrick == BRICK_MODEL_TRANSPARENT_1 || typeBrick == BRICK_MODEL_TRANSPARENT_2)
		return;
	IsDead = true;
}

bool CBrick::IsDetroy()
{
	return isDestroy;
}

ObjectType CBrick::GetItemHolder()
{
	return itemHolder;
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}