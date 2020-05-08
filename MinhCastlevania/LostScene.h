#pragma once
#include "Scene.h"
#include "Font.h"
#include "ScoreBoard.h"

#define CONTINUE_HEART_POSITION	 D3DXVECTOR2(240,190)
#define END_HEART_POSITION		 D3DXVECTOR2(240,230)
#define CONTINUE_POSITION		 D3DXVECTOR2(277,190)
#define END_POSITION			 D3DXVECTOR2(277,230)
#define GAMEOVER_POSITION		 D3DXVECTOR2(240,110)

class CLostScene:public CScene
{
	CFont* font;
	CSprite* heartSel;
	CScoreBoard* csb;
	int isEnter;
public:
	int isContinue;
	CLostScene();
	void Load();
	void Unload();
	void Update(DWORD dt);
	void Render();
	void Enter();
	~CLostScene();
};
class CLostScenceKeyHandler : public CScenceKeyHandler
{
public:
	void KeyState(BYTE* states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	CLostScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
