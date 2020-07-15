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
#include "Sound.h"

#define TIME_SCORE_INCREASE 500
#define TIME_SCORE_TIMEGAME_INCREASE 2
#define TIME_SLEEP_END_GAME 5000
#define TIME_DEFAULT_SCENE 400

class CPlayScene: public CScene
{
private:
	DWORD lifeTime;
	DWORD timescoreincrease;
	int timeGame;
	bool isTransScene;
	string filePathObj;
	string filePathMap;
	int scoreincrease;
	CMap* map;
	CGrid* grid;
	CScoreBoard* cScoreBoard;
	int hpBoss;
	void LoadMap();
	void LoadObject();
	void LoadSound();
	vector<CEffect*> effects;
	D3DXVECTOR2 posDefaultScene;
public:
	CPlayScene(int id, string filePath);
	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();
	void GameOver();
	CMap* GetMap();
	bool IsTransScene();
	void SetScoreIncrease(int _scoreincrease);
	D3DXVECTOR2 GetPosSimonDefault();
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