#pragma once
#include"GameDefine.h"
#include<unordered_map> 

using namespace std;

class CSprite
{
	int id;
	int left; 
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;

public: 
	CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);

	CSprite(int id, LPDIRECT3DTEXTURE9 tex);
	
	void GetInfo(LPDIRECT3DTEXTURE9 tex, int& w, int& h);

	void Draw(float x, float y, int alpha = 255, int R = 255, int G = 255, int B = 255);
	void DrawFlipX(float x, float y, int alpha = 255, int R= 255, int G= 255, int B= 255);
	void DrawFrame(int _x, int _y, RECT rect, int alpha = 255, int R = 255, int G = 255, int B = 255);
	RECT GetRECT();
};
typedef CSprite * LPSPRITE;

class CSprites
{
	static CSprites * __instance;

	unordered_map<int, CSprite*> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	void Add(int id, CSprite* cs);
	LPSPRITE Get(int id);
	void Clear(); 
	void LoadResource();

	static CSprites * GetInstance();
};



