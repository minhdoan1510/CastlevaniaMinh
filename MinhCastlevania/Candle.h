#pragma once
#include"GameObject.h"

#define	CANDLE_1_SIZE D3DXVECTOR2(32,64)
#define	CANDLE_2_SIZE D3DXVECTOR2(16,32)
#define DEATH_ANI_SIZE D3DXVECTOR2(30,30)

class CCandle :public CGameObject
{
	ObjectType itemHolder;
	ObjectType candleType;
	int width;
	int height;
	int IsDeading;
	int IsDestroy;
public:
	CCandle(float x, float y, ObjectType holderType, ObjectType candleType);
	~CCandle();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void SetCandleSize(int _width, int _height);
	void Render();
	void GetBoundingBox(float&l, float&t, float&r, float&b);
	ObjectType GetItemHolder();
	void Death(); 
};