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


class CPlayScene: public CScene
{
private:
	DWORD lifeTime;
	int timeGame;
	int ScoreGame;
	LPCWSTR filePathObj;
	LPCWSTR filePathMap;
	CMap* map;
	CScoreBoard* cScoreBoard;
	vector<LPGAMEOBJECT> objects;
	void LoadMap();
	void LoadObject();
public:
	CPlayScene(int id);
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
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};