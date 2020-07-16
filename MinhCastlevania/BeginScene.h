#pragma once
#include "Scene.h"
#include "Map.h"
#include "Font.h"
#include "Animations.h"
#include "ScoreBoard.h"
#include "GameObject.h"

#define ANI_GAME_POSITION		D3DXVECTOR2(400,170)
#define JOIN_GAME_POSITION		D3DXVECTOR2(170,192)
#define JOIN_GAME_TIME			1000
#define SIMON_ENTER_GAME_TIME	5000
#define FONT_FRAME_TIME			200
#define ANI_BEGIN_SCENE			100
#define DISTANCE_SIMON_AUTO		234

class CBeginScene:public CScene
{
	CMap* mapIntro;
	CFont* font;
	DWORD lTime;
	LPANIMATION aniB;
	//bool isJoinGame;
	CScoreBoard* cScoreBoard;
	bool isIntro;
	vector<CGameObject*>* objects;
public:
	bool isJoinGame;
	CBeginScene(int id = 0, string folderPath = "Resources/IntroScene/BeginScene/");
	void LoadObjectIntroScene();
	void Load();
	void Unload(); 
	void Update(DWORD dt);
	void Render();
	~CBeginScene();
};

class CBeginScenceKeyHandler : public CScenceKeyHandler
{
public:
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	CBeginScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
