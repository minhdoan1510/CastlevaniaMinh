#include "CloudIntro.h"

CCloudIntro::CCloudIntro(float _x, float _y, int _nx)
{
	objType = CLOUD_INTRO;
	x = _x;
	y = _y;
	nx = _nx;
	vx = _nx * CLOUD_INTRO_V_FLY;
	vy = 0;
	sprite = CSprites::GetInstance()->Get(CLOUD_INTRO);
}

CCloudIntro::~CCloudIntro()
{
}

void CCloudIntro::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CLOUD_INTRO_SIZE_W;
	b = y + CLOUD_INTRO_SIZE_H;
}

void CCloudIntro::Render()
{
	RenderBoundingBox();
	sprite->Draw(x, y);
}

void CCloudIntro::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}
