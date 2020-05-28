#pragma once
#include "Scene.h"
#include <unordered_map>
#include <vector>
#include "HandleTransScene.h"

class CSceneManager
{
private:
	std::vector<std::pair<int,LPSCENE>> scenes;
	static CSceneManager* instance;
	int currentSceneID; 
	int currentSceneIDinVector;
	CScene* currentScene;
	CScene* nextScene;
	CMap* nextSceneMap;
	CScene* prevScene;
	bool isPassScene = false;
	CHandleTransScene* cHandleTransScene;
public:
	int ScoreGame;
	static CSceneManager* GetInstance();
	CSceneManager();
	~CSceneManager();
	void AddScene(int id, string pathFolder);
	void SetCurrentSceneID(int id);
	CScene* GetCurrentScene();
	int GetCurrentSceneID();
	//void AddScene(int id);
	//void StartPassScene();
	void EndPassScene();
	void Update(DWORD dt);
	void Render();
	void LostGame();
	void WinGame();
	bool IsPassScene();
	void ReloadScene();
	void StartPassScene(float xDoor=0, float yDoor=0, bool isDebugPass = 0);
	//void ResetScene();
};