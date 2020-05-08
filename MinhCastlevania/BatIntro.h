#pragma once
#include "GameObject.h"

#define BAT_INTRO_V_FLY		0.02
#define BAT_INTRO_SIZE_W	18
#define BAT_INTRO_SIZE_H	18


class CBatIntro :public CGameObject
{
public:
	CBatIntro(float _x, float _y, int _nx);
	~CBatIntro();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
};

