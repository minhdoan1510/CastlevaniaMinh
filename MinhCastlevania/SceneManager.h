#pragma once
#include "Scene.h"
#include <unordered_map>
#include <vector>

class CSceneManager
{
private:
	std::vector<std::pair<int,LPSCENE>> scenes;
	static CSceneManager* instance;
	int currentSceneID; 
	int currentSceneIDinVector;
	CScene* currentScene;
	bool isPassScene = false;
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
	void PassScene();
	void Update(DWORD dt);
	void LostGame();
	void WinGame();
	bool IsPassScene();
	void ReloadScene();
	//void ResetScene();
};