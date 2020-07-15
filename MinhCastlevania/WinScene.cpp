#include "WinScene.h"
#include "Simon.h"
#include "SceneManager.h"

CWinScene::CWinScene() :CScene(-1, "")
{
	key_handler = new CWinSceneKeyHandler(this);
}

void CWinScene::Load()
{
	font = new CFont();
	posWin = WIN_POSITION;
	CCamera::GetInstance()->SetPosition(0,PULL_SCREEN_Y, 1);
	vx = vy = V_FLY;
}

void CWinScene::Unload()
{

}

void CWinScene::Update(DWORD dt)
{
	if (isEnter)
	{
		CSceneManager::GetInstance()->SetCurrentSceneID(0);
		CSimon::GetIntance()->SetFinish(0);
		CSimon::GetIntance()->ResetSimon();
		CSceneManager::GetInstance()->GetCurrentScene()->Load();
		CSceneManager::GetInstance()->GetCurrentScene()->Update(dt);
	}
	if (posWin.x < 0 || posWin.x + WIN_SIZE.x >= CCamera::GetInstance()->GetWidth())
		vx = -vx;
	if (posWin.y < 0 || posWin.y + WIN_SIZE.y >= CCamera::GetInstance()->GetHeight())
		vy = -vy;

	posWin.x += dt * vx;
	posWin.y += dt* vy;
}

void CWinScene::Render()
{
	font->Draw("CONGRATULATIONS", posWin.x, posWin.y);


	if (GetTickCount() - lifeTime >= TIME_LIGHTING&& GetTickCount() - lifeTime<=TIME_LIGHTING*2)
		font->Draw("PRESS ENTER TO ENDGAME", ENTER_POSITION.x, ENTER_POSITION.y);
	else if(GetTickCount() - lifeTime > TIME_LIGHTING * 2)
	{
		lifeTime = GetTickCount();
	}
}

CWinScene::~CWinScene()
{
}

void CWinSceneKeyHandler::KeyState(BYTE* states)
{

}

void CWinSceneKeyHandler::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_RETURN:
		static_cast<CWinScene*>(CSceneManager::GetInstance()->GetCurrentScene())->isEnter=true;
		break;
	}
}

void CWinSceneKeyHandler::OnKeyUp(int KeyCode)
{

}
