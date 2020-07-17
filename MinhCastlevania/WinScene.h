#pragma once
#include "Scene.h"
#include "Font.h"
#include "ScoreBoard.h"

#define MUSIC_FOLDER		"Resources/Sound/musicMap1.wav"
#define ENTER_POSITION		D3DXVECTOR2(90,400)
#define WIN_POSITION		D3DXVECTOR2(63,110)
#define WIN_SIZE			D3DXVECTOR2(240,60)
#define TIME_LIGHTING		500
#define V_FLY				.1f

class CWinScene :public CScene
{
	CFont* font;
	DWORD lifeTime;
	D3DXVECTOR2 posWin;
	float vx, vy;
public:
	bool isEnter;
	int isContinue;
	CWinScene();
	void LoadMusic();
	void Load();
	void Unload();
	void Update(DWORD dt);
	void Render();
	~CWinScene();
};
class CWinSceneKeyHandler : public CScenceKeyHandler
{
public:
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	CWinSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
