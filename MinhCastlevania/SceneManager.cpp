#include "SceneManager.h"
#include "PlayScence.h"

CSceneManager* CSceneManager::instance = NULL;

CSceneManager::CSceneManager()
{
	currentSceneID = 0;
	LPANIMATION_SET temp = CAnimationSets::GetInstance()->Get(SIMON);
	CSimon::GetIntance()->SetAnimationSet(temp);
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::AddScene(int id)
{
	scenes.push_back(new CPlayScene(id));
	if (scenes.size() == 1)
	{
		currentScene = scenes[0];
		CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
		currentScene->Load();
	}
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
	currentScene->Update(dt);
}

void CSceneManager::LostGame()
{
	///đang làm
}

bool CSceneManager::IsPassScene()
{
	if (isPassScene){
		isPassScene = 0;
		return true;
	}
	return false;
}

void CSceneManager::ReloadScene()
{
	SAFE_DELETE(currentScene);
	currentScene = new CPlayScene(currentSceneID+1);
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
	currentScene->Load();
}


void CSceneManager::PassScene()
{
	currentSceneID++;
	currentScene->Unload();
	SAFE_DELETE(currentScene);
	currentScene = scenes.at(currentSceneID);
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
	currentScene->Load();
	float _x, _y;
	CSimon::GetIntance()->GetPosition(_x, _y);
	CMapManager::GetIntance()->Get(currentSceneID)->SetBoundaryLeftRight(1 + (int)(_y + SIMON_BBOX_HEIGHT / 2) / (CMapManager::GetIntance()->Get(currentSceneID)->GetMapHeight() / CMapManager::GetIntance()->Get(currentSceneID)->GetFloorMap()));
	isPassScene = true;
}

CScene* CSceneManager::GetCurrentScene()
{
	if (currentScene == nullptr)
	{
		currentScene = scenes.at(currentSceneID);
		CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
	}
	return this->currentScene;
}

int CSceneManager::GetCurrentSceneID()
{
	return currentSceneID;
}