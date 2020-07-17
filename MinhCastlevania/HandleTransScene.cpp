#include "HandleTransScene.h"
#include "Simon.h"
#include "PlayScence.h"



CHandleTransScene::CHandleTransScene(CMap* pre, CMap* next, int direct, D3DXVECTOR2 _posSimonNextMap, D3DXVECTOR2 _posDoorScene)
{
	preMap = pre;
	nextMap = next;
	directTrans = direct; 
	float xtemp, ytemp;
	CSimon::GetIntance()->GetPosition(xtemp, ytemp);
	posSimonNextMap = _posSimonNextMap;
	posDoorScene = _posDoorScene;

	float _l, _r;
	CCamera::GetInstance()->GetBoundLeftRight(_l, _r);
	if (directTrans == -1)
	{
		//posNextMap.x = -next->GetMapWidth();
		posNextMap.x = -next->GetMapWidth();
		posNextMap.y = -(_posSimonNextMap.y - ytemp);
		distanceSimon = abs(_posSimonNextMap.x + posNextMap.x - xtemp);
	}
	else
	{
		posNextMap.x = pre->GetMapWidth();
		posNextMap.y = -(_posSimonNextMap.y - ytemp);
		distanceSimon = abs(posNextMap.x + _posSimonNextMap.x - xtemp);
	}
	Init();
}

CHandleTransScene::~CHandleTransScene()
{
}

void CHandleTransScene::Init()
{
	aniDoor = CAnimationSets::GetInstance()->Get(DOOR_SCENE);
	aniDoorState = DOOR_STATE_CLOSE_SCENE;
	transCam2nd = 0;
	isPassDoorScene = 0;
	DoneTransScene = 0;
	cScoreBoard = new CScoreBoard();
	CCamera::GetInstance()->AutoCamX(-PULL_X_SCREEN + (int)SCREEN_WIDTH / 2,CSimon::GetIntance()->GetNx());
	cScoreBoard->Update(
		TIME_DEFAULT_SCENE, 
		CSceneManager::GetInstance()->ScoreGame,
		CSimon::GetIntance()->GetHeart(), 
		CSimon::GetIntance()->GetLifeSimon(), 
		CSimon::GetIntance()->GetHPSimon(), 
		SIMON_DEFAULT_HP,
		CSceneManager::GetInstance()->GetCurrentSceneID()+1, 
		CSimon::GetIntance()->GetSecondWeapon(),
		CSimon::GetIntance()->GetAmount2ndWeapon());
}

void CHandleTransScene::Update(DWORD dt)
{
	if (!CCamera::GetInstance()->IsAutoCam())
	{
		DWORD now = GetTickCount();
		if ((aniDoorState == DOOR_STATE_CLOSE_SCENE || aniDoorState == DOOR_STATE_OPEN_SCENE) && transCam2nd ==0)
		{
			if (isGoPassScene == 1)
			{
				if (!CSimon::GetIntance()->IsAutoGo()&& isPassDoorScene==0)
				{
					aniDoorState = DOOR_CLOSE_SCENE;
					timeDoor = now;
				}

				if(isPassDoorScene)
				{
					CCamera::GetInstance()->AutoCamX(-PULL_X_SCREEN + (int)SCREEN_WIDTH / 2, CSimon::GetIntance()->GetNx());
					transCam2nd = 1;
				}
			}
			else
				if (DOOR_STATE_CLOSE_SCENE)
				{
					aniDoorState = DOOR_OPEN_SCENE;
					timeDoor = now;
				}
		}
		else
		{
			if (now - timeDoor >= DOOR_ANI_TIME && timeDoor != 0)
			{
				timeDoor = 0;
				if (aniDoorState == DOOR_CLOSE_SCENE)
				{
					aniDoorState = DOOR_STATE_CLOSE_SCENE;
					isPassDoorScene = 1;
				}
				else
				{
					aniDoorState = DOOR_STATE_OPEN_SCENE;
					if (isGoPassScene == 0 && !CSimon::GetIntance()->IsAutoGo())
					{
						isGoPassScene = 1;
						CSimon::GetIntance()->SetAutoGo(distanceSimon, directTrans, directTrans);
					}
				}
			}
			else
			{
				if (transCam2nd)
				{
					DoneTransScene = 1;
					return;
				}
				
				if (timeDoor == 0 && isGoPassScene == 1 && !CSimon::GetIntance()->IsAutoGo())
				{
					aniDoorState = DOOR_CLOSE_SCENE;
					timeDoor = now;
				}
			}
		}
	}
	else
	{
		CCamera::GetInstance()->UpdateAutoCam(dt);
	}
	if (CSimon::GetIntance()->IsAutoGo())
	{
		aniDoorState = DOOR_STATE_OPEN_SCENE;
		CSimon::GetIntance()->UpdateAutoGo(dt);
	}
}

void CHandleTransScene::Render()
{
	preMap->DrawMap();
	nextMap->DrawMapTransform(-posNextMap.x, -posNextMap.y, directTrans);
	cScoreBoard->Render();

	if (aniDoorState == DOOR_CLOSE_SCENE || aniDoorState == DOOR_OPEN_SCENE)
		aniDoor->at(aniDoorState)->Render(posDoorScene.x, posDoorScene.y, directTrans == 1);
	else
	{
		switch (aniDoorState)
		{
		case DOOR_STATE_CLOSE_SCENE:
			aniDoor->at(DOOR_CLOSE_SCENE)->RenderFrame(aniDoor->at(DOOR_CLOSE_SCENE)->GetLastFrame(), posDoorScene.x, posDoorScene.y, directTrans == 1);
			break;
		case DOOR_STATE_OPEN_SCENE:
			aniDoor->at(DOOR_OPEN_SCENE)->RenderFrame(aniDoor->at(DOOR_OPEN_SCENE)->GetLastFrame(), posDoorScene.x, posDoorScene.y, directTrans == 1);
			break;
		}
	}
	CSimon::GetIntance()->Render();
}

bool CHandleTransScene::IsDoneTransScene()
{
	return DoneTransScene;
}
