#pragma once
#include"Map.h"
#include "ScoreBoard.h"

#define PULL_X_SCREEN 8

#define DOOR_ANI_TIME	 450 //150*2

#define DOOR_IDLE_SCENE			9610
#define DOOR_OPEN_SCENE			9620
#define DOOR_STATE_OPEN_SCENE	9621
#define DOOR_CLOSE_SCENE		9630
#define DOOR_STATE_CLOSE_SCENE	9631

class CHandleTransScene
{
	DWORD timeDoor;
	CMap* preMap, * nextMap;
	int directTrans;
	float distanceSimon;
	D3DXVECTOR2 posSimonNextMap;
	D3DXVECTOR2 posNextMap;
	D3DXVECTOR2 posDoorScene;
	bool transCam2nd;
	bool DoneTransScene;
	bool isGoPassScene;
	bool isPassDoorScene;
	CScoreBoard* cScoreBoard;
	LPANIMATION_SET aniDoor;
	int aniDoorState;	
public:
	CHandleTransScene(CMap* pre, CMap* next, int direct, D3DXVECTOR2 _posSimonNextMap, D3DXVECTOR2 _posDoorScene);
	~CHandleTransScene();
	void Init();
	void Update(DWORD dt);
	void Render();
	bool IsDoneTransScene();
};

