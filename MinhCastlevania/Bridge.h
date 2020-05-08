#pragma once
#include"GameObject.h"


#define BRIDGE_BBOX_WIDTH			63	
#define BRIDGE_BBOX_HEIGHT			16
#define BRIDGE_SPEED				0.045f
#define BRIDGE_FOOT_SIMON			15

class CBridge :public CGameObject
{
public:
	CBridge(float _x, float _y);
	~CBridge();
	void Render();
	void ChangeDirect();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

