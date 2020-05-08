#include "BatIntro.h"

CBatIntro::CBatIntro(float _x,float _y, int _nx)
{
	objType = BAT_INTRO;
	x = _x;
	y = _y;
	nx = _nx;
	vx = _nx * BAT_INTRO_V_FLY;
	vy = -BAT_INTRO_V_FLY;
	ani = CAnimations::GetInstance()->Get(BAT_INTRO);
}

CBatIntro::~CBatIntro()
{
}

void CBatIntro::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BAT_INTRO_SIZE_W;
	b = y + BAT_INTRO_SIZE_H;
}

void CBatIntro::Render()
{
	RenderBoundingBox();
	ani->Render(x, y, 0);
}

void CBatIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}
