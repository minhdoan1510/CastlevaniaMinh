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


#define TIME_SCORE_INCREASE 100
#define TIME_SCORE_TIMEGAME_INCREASE 2
#define TIME_SLEEP_END_GAME 5000
#define TIME_DEFAULT_SCENE 400

#define SPRITE_FILE  "/sprite.txt"
#define TEXTURE_FILE "/texture.txt"
#define FOLDER_MAP	"Resources/map/"
#define BACKMUSIC_FILE	"/musicMap.wav"
#define BOSSMUSIC_FILE	"/bossmusic.wav"
#define ENDMUSIC_FILE	"/endmusic.wav"
#define MUSIC_NAME	"MusicMap"

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
	bool isLoadBossMusic;
	bool isLoadEndMusic;
	bool isIncreaseScoreMusic;
	CGrid* grid;
	CScoreBoard* cScoreBoard;
	int hpBoss;
	vector<CEffect*> effects;
	D3DXVECTOR2 posDefaultScene;
	void LoadMap();
	void LoadObject();
	void LoadSound();
	void LoadBossMusic();
	void LoadEndMusic();
	void LoadIncreaseScoreMusic();
public:
	CPlayScene(int id, string filePath);
	void Load();
	void ResumeMusic();
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