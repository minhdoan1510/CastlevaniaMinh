#pragma once
#include"GameObject.h"

#define CLOUD_INTRO_V_FLY		0.01
#define CLOUD_INTRO_SIZE_W		65
#define CLOUD_INTRO_SIZE_H		30

class CCloudIntro :public CGameObject
{
public:
	CCloudIntro(float _x, float _y, int _nx);
	~CCloudIntro();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
};

