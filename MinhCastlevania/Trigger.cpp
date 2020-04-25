#include "Trigger.h"
#include "Simon.h"

CTrigger::CTrigger(int _x, int _y, int _w, int _h, ObjectType _type, int _DirectStair)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;
	triggerType = _type;
	if (_DirectStair == 1)
		x_checkpoint = x;
	else
		x_checkpoint = x + 32;
	DirectStair = _DirectStair;
	//isColisible = false;
}

CTrigger::~CTrigger()
{
}

void CTrigger::Render()
{
	RenderBoundingBox();
}

void CTrigger::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = width + l;
	b = height + t;
}
