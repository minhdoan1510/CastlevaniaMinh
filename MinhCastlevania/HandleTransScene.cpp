#include "HandleTransScene.h"
#include "Simon.h"
#include "PlayScence.h"



CHandleTransScene::CHandleTransScene(CMap* pre, CMap* next, int direct, D3DXVECTOR2 _posSimonNextMap)
{
	preMap = pre;
	nextMap = next;
	directTrans = direct; 
	float xtemp, ytemp;
	CSimon::GetIntance()->GetPosition(xtemp, ytemp);
	posSimonNextMap = _posSimonNextMap;

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
	transCam2nd = 0;
	DoneTransScene = 0;
	cScoreBoard = new CScoreBoard();
	CCamera::GetInstance()->AutoCamX(-PULL_X_SCREEN + (int)SCREEN_WIDTH / 2,CSimon::GetIntance()->GetNx());
	cScoreBoard->Update(TIME_DEFAULT_SCENE, CSceneManager::GetInstance()->ScoreGame, CSimon::GetIntance()->GetHeart(), CSimon::GetIntance()->GetLifeSimon(), CSimon::GetIntance()->GetHPSimon(), 16, CSceneManager::GetInstance()->GetCurrentSceneID()+1, CSimon::GetIntance()->GetSecondWeapon(), CSimon::GetIntance()->GetAmount2ndWeapon());
}

void CHandleTransScene::Update(DWORD dt)
{
	if (!CCamera::GetInstance()->IsAutoCam())
	{
		if (transCam2nd)
		{
			DoneTransScene = 1;
			return;
		}
		if (isGoPassScene == 0&&!CSimon::GetIntance()->IsAutoGo())
		{
			isGoPassScene = 1;
			CSimon::GetIntance()->SetAutoGo(distanceSimon, directTrans, directTrans);
		}
		if(isGoPassScene == 1&&!CSimon::GetIntance()->IsAutoGo())
		{
			CCamera::GetInstance()->AutoCamX(-PULL_X_SCREEN + (int)SCREEN_WIDTH / 2, CSimon::GetIntance()->GetNx());
			transCam2nd = 1;
		}
	}
	else
	{
		CCamera::GetInstance()->UpdateAutoCam(dt);
	}
	if (CSimon::GetIntance()->IsAutoGo())
		CSimon::GetIntance()->UpdateAutoGo(dt);
	
}

void CHandleTransScene::Render()
{
	preMap->DrawMap();
	nextMap->DrawMapTransform(-posNextMap.x, -posNextMap.y, directTrans);
	CSimon::GetIntance()->Render();
	cScoreBoard->Render();
}

bool CHandleTransScene::IsDoneTransScene()
{
	return DoneTransScene;
}
