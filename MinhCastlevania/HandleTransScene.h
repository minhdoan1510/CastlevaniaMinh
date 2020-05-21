#pragma once
#include"Map.h"
#include "ScoreBoard.h"

#define PULL_X_SCREEN 8

class CHandleTransScene
{
	CMap* preMap, * nextMap;
	int directTrans;
	float distanceSimon;
	D3DXVECTOR2 posSimonNextMap;
	D3DXVECTOR2 posNextMap;
	bool transCam2nd;
	bool DoneTransScene;
	bool isGoPassScene;
	CScoreBoard* cScoreBoard;
public:
	CHandleTransScene(CMap* pre, CMap* next , int direct, D3DXVECTOR2 _posSimonNextMap);
	~CHandleTransScene();
	void Init();
	void Update(DWORD dt);
	void Render();
	bool IsDoneTransScene();
};

