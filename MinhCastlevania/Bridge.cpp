#include "Bridge.h"
#include "Simon.h"
#include "Brick.h"

CBridge::CBridge(float _x, float _y)
{
	nx = 1;
	objType = BRIDGE;
	sprite = CSprites::GetInstance()->Get(BRIDGE);
	vx = BRIDGE_SPEED;
	vy = 0;
	x = _x;
	y = _y;
}

CBridge::~CBridge()
{
}

void CBridge::Render()
{
	sprite->Draw(x, y);
}

void CBridge::ChangeDirect()
{
	nx = -nx;
	vx = nx * BRIDGE_SPEED;
}

void CBridge::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	vector<CGameObject*>* colliable_objects = new vector<CGameObject*>(); // AddSimon

	for (int i = 0; i < objects->size(); i++)
		colliable_objects->push_back(objects->at(i));
	colliable_objects->push_back(CSimon::GetIntance());

	CalcPotentialCollisions(colliable_objects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		//y += dy;
	}
	else
	{
		float min_tx, min_ty, _nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, _nx, ny);
		bool flagBrick = 0;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			if (dynamic_cast<CBrick*>(coEventsResult.at(i)->obj))
			{
				flagBrick = 1;
			}
		}

		if (ny != 0)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				if (coEventsResult.at(i)->ny != 0)
				{
					float xtemp, ytemp;
					coEventsResult.at(i)->obj->GetPosition(xtemp, ytemp);
					//if ((abs((x+BRIDGE_BBOX_WIDTH)- xtemp) >= BRIDGE_FOOT_SIMON&&  xtemp -x > 0)||(abs(x+SIMON_BBOX_WIDTH - xtemp) >= BRIDGE_FOOT_SIMON && xtemp -x <=0))
					//{
						xtemp += min_tx * dx + _nx * 0.4f;
						coEventsResult.at(i)->obj->SetPosition(xtemp, ytemp);
					//}
				}
			}
			vy = 0;
		}


		if (flagBrick)
			x += min_tx * dx + _nx * 0.4f;
		else
			x += dx;
		//y += min_ty * dy + ny * 0.4f;

		if (_nx != 0 && flagBrick)
		{
			ChangeDirect();
		}
	}
}

void CBridge::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRIDGE_BBOX_WIDTH;
	b = y + BRIDGE_BBOX_HEIGHT;
}
