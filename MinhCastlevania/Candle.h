#pragma once
#include"GameObject.h"

#define	CANDLE_1_SIZE_W 32
#define	CANDLE_1_SIZE_H 64
#define	CANDLE_2_SIZE_W 16
#define	CANDLE_2_SIZE_H 32

class CCandle :public CGameObject
{
	ObjectType itemHolder;
	ObjectType candleType;
	int width;
	int height;
	int IsDeading;
	int IsDestroy;
	CAnimation* ani;

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