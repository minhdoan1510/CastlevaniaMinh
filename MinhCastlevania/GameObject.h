#pragma once
#include <vector>

#include "Sprites.h"
#include "GameDefine.h"
#include "Animations.h"

using namespace std;

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;	
	float dx, dy;
	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}
	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};

class CGameObject
{
protected:
	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	

	DWORD dt; 

	bool IsDead = false;
	bool isColisible = true;
	bool IsFinish = false;

	LPANIMATION_SET animation_set;
	LPANIMATION ani;
	ObjectType objType;

	LPSPRITE sprite;
	LPDIRECT3DTEXTURE9 texture;
public: 
	virtual void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	D3DXVECTOR2 GetPosition() { return D3DXVECTOR2(this->x, this->y); }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	int GetNx() { return nx; }
	bool GetDeath();
	virtual void Death() {}
	bool IsContain(RECT rect1, RECT rect2);	
	void SetAnimationSet(LPANIMATION_SET ani_set);	
	LPANIMATION GetAni();

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny);

	RECT GetBBox();
	void RenderBoundingBox();
	virtual ObjectType GetItemHolder() 
	{
		return Null; 
	}
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	ObjectType GetObjType() { return objType; }
	CGameObject();
	~CGameObject();
};

