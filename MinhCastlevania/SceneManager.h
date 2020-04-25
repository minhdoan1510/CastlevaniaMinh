#pragma once
#include "Scene.h"
#include <vector>

class CSceneManager
{
private:
	std::vector<LPSCENE> scenes;
	static CSceneManager* instance;
	int currentSceneID;
	CScene* currentScene;
	bool isPassScene = false;
public:
	static CSceneManager* GetInstance();
	CSceneManager();
	~CSceneManager();
	CScene* GetCurrentScene();
	int GetCurrentSceneID();
	void AddScene(int id);
	void PassScene();
	void Update(DWORD dt);
	void LostGame();
	bool IsPassScene();
	void ReloadScene();
};