#include "Trigger.h"
#include "Simon.h"
#include "SceneManager.h"

CTrigger::CTrigger(int _x, int _y, int _w, int _h, ObjectType _type, int _DirectStair)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;
	triggerType = _type;

	if (triggerType == PASS_SCENE_TRIGGER)
		spriteDoor = CSprites::GetInstance()->Get(9601);

	if (_DirectStair == 1)
		x_checkpoint = x + CHECKPOINT_X1;
	else
		x_checkpoint = x - CHECKPOINT_X;
	DirectStair = _DirectStair;
	isActive = 1;
	//isColisible = false;
}

CTrigger::~CTrigger()
{
}

void CTrigger::Render()
{
	RenderBoundingBox();
	if (triggerType == PASS_SCENE_TRIGGER&&CSceneManager::GetInstance()->GetCurrentSceneID()!=1)
	{
		if (DirectStair == 1)
			spriteDoor->DrawFlipX(x, y);
		else
			spriteDoor->Draw(x, y);
	}
}

void CTrigger::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y + ((triggerType == PASS_SCENE_TRIGGER)? HEIGHT_DOORPASS : 0);
	r = width + l;
	b = height + t;
}

