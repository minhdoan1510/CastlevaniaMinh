#pragma once
#include "Scene.h"
#include "Font.h"
#include "ScoreBoard.h"

#define CONTINUE_HEART_POSITION	 D3DXVECTOR2(210,190)
#define END_HEART_POSITION		 D3DXVECTOR2(210,230)
#define ENTER_POSITION		 D3DXVECTOR2(247,190)
#define END_POSITION			 D3DXVECTOR2(247,230)
#define WIN_POSITION		 D3DXVECTOR2(210,110)

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
