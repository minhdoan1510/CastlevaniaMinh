#pragma once
#include"GameObject.h"

#define TIME_DEAD_EFFECT 1000

class CEffect : public CGameObject
{
protected:
	ObjectType effectType;
	DWORD lifeTime;
	int numDraw;
public:
	CEffect(float _x, float _y, ObjectType type);
	~CEffect();
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};