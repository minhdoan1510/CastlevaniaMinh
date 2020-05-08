#include "Candle.h"
#include"TextureManager.h"

CCandle::CCandle(float x, float y, ObjectType _itemHolder, ObjectType _candleType)
{
	itemHolder = _itemHolder;
	SetPosition(x, y);
	candleType = _candleType;
	objType = CANDLE;
	isColisible = 0;
	IsDeading = false;
	IsDestroy = false;
	ani = CAnimationSets::GetInstance()->Get(CANDLE)->at(candleType);
	//ani = CAnimationSets::GetInstance()->Get(CANDLE)->at(candleType)->Clone();
}

CCandle::~CCandle()
{
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	switch (candleType)
	{
	case CANDLE1:
		width = CANDLE_1_SIZE.x;
		height = CANDLE_1_SIZE.y;
		break;
	case CANDLE2:
		width = CANDLE_2_SIZE.x;
		height = CANDLE_2_SIZE.y;
		break;
	case DEATH_ANI:
		width = DEATH_ANI_SIZE.x;
		height = DEATH_ANI_SIZE.y;
		break;
	}
}
void CCandle::SetCandleSize(int _width, int _height)
{
	width = _width;
	height = _height;
}
void CCandle::Render()
{
	if (IsDead) return;
	if (IsDestroy == false)
	{
		ani->Render(x,y,false);
	}
	else
	{
		if (!IsDeading)
		{
			ani = CAnimations::GetInstance()->Get(DEATH_ANI);
			candleType = DEATH_ANI;
			IsDeading = true;
		}
		else
		{
			ani->Render(x, y, false);
			if (ani->IsFinalFrame() == true)
			{
				IsDead = true;				
			}
		}
	}
	RenderBoundingBox();
}
void CCandle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = width + l;
	b = height + t;
}
ObjectType CCandle::GetItemHolder()
{
	return  itemHolder;
}
void CCandle::Death()
{
	IsDestroy = true;
}