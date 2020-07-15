#include "SceneManager.h"
#include "PlayScence.h"
#include "BeginScene.h"
#include "LostScene.h"
#include "WinScene.h"

CSceneManager* CSceneManager::instance = NULL;

CSceneManager::CSceneManager()
{
	currentSceneID = 0;
	currentSceneIDinVector = -1;
	ScoreGame = 0;
	scenes.push_back(make_pair(0, new CBeginScene()));
	cHandleTransScene = nullptr;
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::AddScene(int id, string pathFolder)
{
	scenes.push_back(make_pair(id, new CPlayScene(id, pathFolder)));

	/*if (scenes.size() == 1)
	{
		currentScene = scenes[0];
		CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
		currentScene->Load();
	}*/
}

void CSceneManager::SetCurrentSceneID(int id)
{
	//if (dynamic_cast<CLostScene*> (currentScene))
		//SAFE_DELETE(currentScene);
	if (id == 1 || id == 0)
	{
		CSimon::GetIntance()->ResetLifeSimon();
		ScoreGame = 0;
	}for (int i = 0; i < scenes.size(); i++)
	{
		if (scenes.at(i).first == id)
		{
			currentSceneIDinVector = i;
			currentScene = scenes.at(i).second;
			currentSceneID = id;
			break;
		}
	}
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
	currentScene->Load();
	if (currentSceneIDinVector == -1)
		DebugOut(L"[SceneManager] Not found scene id: &d\n", id);

}

CSceneManager* CSceneManager::GetInstance()
{
	if (!instance)
	{
		instance = new CSceneManager();
	}
	return instance;
}

void CSceneManager::Update(DWORD dt)
{
	if (isPassScene)
	{
		if (cHandleTransScene == NULL || cHandleTransScene->IsDoneTransScene())
		{
			isPassScene = 0;
			EndPassScene();
			currentScene->Update(dt);
			return;
		}
		else
		{
			cHandleTransScene->Update(dt);
		}
	}
	else
	{
		currentScene->Update(dt);
	}
}

void CSceneManager::Render()
{
	if (isPassScene)
	{
		cHandleTransScene->Render();
	}
	else
	{
		currentScene->Render();
	}
}

void CSceneManager::LostGame()
{
	CCamera::GetInstance()->UnlockCam();
	currentScene->Unload();
	currentScene = new CLostScene();
	currentScene->Load();
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
}

void CSceneManager::WinGame()
{
	///đang làm
	CCamera::GetInstance()->UnlockCam();
	currentScene->Unload();
	currentScene = new CWinScene();
	currentScene->Load();
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
}

bool CSceneManager::IsPassScene()
{
	if (isPassScene){
		//isPassScene = 0;
		return true;
	}
	return false;
}

void CSceneManager::ReloadScene()
{
	string folderpath = currentScene->GetFolderPath();
	//SAFE_DELETE(currentScene);
	currentScene = new CPlayScene(currentSceneID, folderpath);
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
	ScoreGame = 0;
	currentScene->Load();
	D3DXVECTOR2 PosSimon = static_cast<CPlayScene*>(currentScene)->GetPosSimonDefault();
	CSimon::GetIntance()->SetPosition(PosSimon.x, PosSimon.y);
}

void CSceneManager::StartPassScene(float xDoor, float yDoor, bool isDebugPass)
{
	if (currentSceneIDinVector + 1 >= scenes.size())
	{
		WinGame();
		return;
	}
	
	nextScene = scenes.at(currentSceneIDinVector+1).second;
	
	isPassScene = 1;
	if (currentSceneID == 1||isDebugPass)
	{
		currentScene->Unload();
		//SAFE_DELETE(currentScene);
		currentScene = nextScene;
		currentScene->Load();
		return;
	}
	nextScene->Load();
	cHandleTransScene = new CHandleTransScene(static_cast<CPlayScene*>(currentScene)->GetMap(), static_cast<CPlayScene*>(nextScene)->GetMap(), CSimon::GetIntance()->GetNx(), static_cast<CPlayScene*>(nextScene)->GetPosSimonDefault(),D3DXVECTOR2(xDoor,yDoor));
}

void CSceneManager::EndPassScene()
{
	
	currentSceneIDinVector++;
	currentScene = nextScene;
	currentSceneID = scenes.at(currentSceneIDinVector).first;
	//prevScene = currentScene;
	if (currentSceneID != 1)
	{
		currentScene->Unload();
		//SAFE_DELETE(currentScene);
		currentScene = scenes.at(currentSceneIDinVector).second;
		currentScene->Load();
	}
	
	//currentScene = scenes.at(currentSceneIDinVector).second;
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
	//currentScene->Load();

	D3DXVECTOR2 PosSimon = static_cast<CPlayScene*>(currentScene)->GetPosSimonDefault();
	CSimon::GetIntance()->SetPosition(PosSimon.x, PosSimon.y);

	float _x, _y;
	CSimon::GetIntance()->GetPosition(_x, _y);
	CMapManager::GetIntance()->Get(currentSceneID)->SetBoundaryLeftRight(1 + (int)(_y + SIMON_BBOX_HEIGHT / 2) / (CMapManager::GetIntance()->Get(currentSceneID)->GetMapHeight() / CMapManager::GetIntance()->Get(currentSceneID)->GetFloorMap()));
	isPassScene = 0;
	SAFE_DELETE(cHandleTransScene);
}

CScene* CSceneManager::GetCurrentScene()
{
	if (currentScene == nullptr)
	{
		currentScene = scenes.at(currentSceneIDinVector).second;
		CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
	}
	currentSceneID = scenes.at(currentSceneIDinVector).first;
	return currentScene;
}

int CSceneManager::GetCurrentSceneID()
{
	return currentSceneID;
}