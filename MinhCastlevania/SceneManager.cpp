#include "SceneManager.h"
#include "PlayScence.h"
#include "BeginScene.h"
#include "LostScene.h"

CSceneManager* CSceneManager::instance = NULL;

CSceneManager::CSceneManager()
{
	currentSceneID = 0;
	currentSceneIDinVector = -1;
	ScoreGame = 0;
	scenes.push_back(make_pair(0, new CBeginScene()));
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
	for (int i = 0; i < scenes.size(); i++)
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
	currentScene->Update(dt);
}

void CSceneManager::LostGame()
{
	currentScene->Unload();
	currentScene = new CLostScene();
	currentScene->Load();
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
}

void CSceneManager::WinGame()
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
	string folderpath = currentScene->GetFolderPath();
	//SAFE_DELETE(currentScene);
	currentScene = new CPlayScene(currentSceneID, folderpath);
	CGame::GetInstance()->SetKeyHandler(currentScene->GetKeyEventHandler());
	ScoreGame = 0;
	currentScene->Load();
}


void CSceneManager::PassScene()
{
	currentSceneIDinVector++;
	currentScene->Unload();
	//SAFE_DELETE(currentScene);
	if (currentSceneIDinVector >= scenes.size())
	{
		WinGame();
		return;
	}
	currentSceneID = scenes.at(currentSceneIDinVector).first;
	currentScene = scenes.at(currentSceneIDinVector).second;
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