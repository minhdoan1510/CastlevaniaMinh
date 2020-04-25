#include <d3dx9.h>
#include <algorithm>
#include "Utils.h"
#include "TextureManager.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Simon.h"
#include "Trigger.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

RECT CGameObject::GetBBox()
{
	float left, top, right, bottom;
	GetBoundingBox(left,  top,  right,  bottom);
	RECT Bbox;
	Bbox.left = (long)left;
	Bbox.top = (long)top;
	Bbox.right = (long)right;
	Bbox.bottom = (long)bottom;
	return  Bbox;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
}

LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;
	float ml, mt, mr, mb;	
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dx, dy, coO);
	return e;
}

void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents) {

	for (int i = 0; i < coObjects->size(); i++)
	{
		if(dynamic_cast<CTrigger*>(coObjects->at(i)))
			continue;
		if((dynamic_cast<CTrigger*>(coObjects->at(i)) && dynamic_cast<CSimon*>(this)))
		{ }
		else
		if ((!coObjects->at(i)->isColisible && !dynamic_cast<CWeapon*> (this)))//&& dynamic_cast<CSimon*>(this)) 
			continue;
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty, 
	float &nx, float &ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix>=0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy>=0) coEventsResult.push_back(coEvents[min_iy]);
}

bool CGameObject::GetDeath()
{
	return IsDead;
}

void CGameObject::RenderBoundingBox()
{
	RECT rect = GetBBox();

	LPDIRECT3DTEXTURE9 bbox = CTextureManager::GetInstance()->Get(BBOX);
	if (!IsContain(rect, CCamera::GetInstance()->GetRectCam()))
		return;
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = rect.right - rect.left;
	r.bottom = rect.bottom - rect.top;
	D3DXVECTOR2 pos = CCamera::GetInstance()->Transform(rect.left, rect.top + PULL_SCREEN_Y);
	CGame::GetInstance()->GetSpriteHandler()->Draw(bbox, &r, NULL, &D3DXVECTOR3(pos.x, pos.y, 0), D3DCOLOR_ARGB(128, 255, 255, 255));
}

bool CGameObject::IsContain(RECT rect1, RECT rect2)
{
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}
	return true;
}

void CGameObject::SetAnimationSet(LPANIMATION_SET ani_set)
{
	animation_set = ani_set;
}

CGameObject::~CGameObject()
{
}