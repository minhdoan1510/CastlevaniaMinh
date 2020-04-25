#include "Brick.h"
#include "TextureManager.h"
#include "GameDefine.h"

CBrick::CBrick(float X, float Y, int W, int H, int _type)
{
	typeBrick = static_cast<ObjectType>(_type);
	this->x = X;
	this->y = Y;
	this->width = W;
	this->height = H;
	objType = ObjectType::BRICK;
	if (typeBrick == BRICK_MODEL_TRANSPARENT_1 || typeBrick == BRICK_MODEL_TRANSPARENT_2) return;
	sprite = CSprites::GetInstance()->Get(typeBrick);
	if (sprite == NULL)
		DebugOut(L"[Brick] Sprite brick type %d not found in CSprites\n",_type);
}

void CBrick::Render()
{
	RenderBoundingBox();
	if (typeBrick == BRICK_MODEL_TRANSPARENT_1)
		return;
	sprite->Draw(x, y);
}

int CBrick::GetTypeBrick()
{
	return typeBrick;
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}