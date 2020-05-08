#pragma once
#include "Game.h"
#include "TextureManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Simon.h"
#include "Camera.h"
#include "MapManager.h"
#include "Brick.h"
#include "Candle.h"
#include "Item.h"
#include "ScoreBoard.h"
#include "Grid.h"
#include "Effect.h"

#define TIME_DEFAULT_SCENE 400

class CPlayScene: public CScene
{
private:
	DWORD lifeTime;
	int timeGame;
	string filePathObj;
	string filePathMap;
	CMap* map;
	CGrid* grid;
	CScoreBoard* cScoreBoard;
	void LoadMap();
	void LoadObject();
	vector<CEffect*> effects;
public:
	CPlayScene(int id, string filePath);
	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();
	void GameOver();
	friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	void KeyState(BYTE *states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};