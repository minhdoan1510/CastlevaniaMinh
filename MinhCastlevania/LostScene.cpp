#include "LostScene.h"
#include "Simon.h"
#include "SceneManager.h"
#include "Sound.h"

void CLostScene::LoadMusic()
{
	CSound::GetInstance()->stop("MusicMap");
	CSound::GetInstance()->loadSound(MUSIC_FOLDER, "MusicMap");
}

CLostScene::CLostScene():CScene(-1,"")
{
	key_handler = new CLostScenceKeyHandler(this);
}

void CLostScene::Load()
{
	isContinue = 1;
	font = new CFont();
	heartSel = CSprites::GetInstance()->Get(HEART_ITEM_BOARD);
	csb = new CScoreBoard();
	isEnter = 0;
	csb->Update(
		TIME_GAME,
		CSceneManager::GetInstance()->ScoreGame, 
		CSimon::GetIntance()->GetHeart(), 
		CSimon::GetIntance()->GetLifeSimon(),
		CSimon::GetIntance()->GetHPSimon(), 
		SIMON_DEFAULT_HP,
		CSceneManager::GetInstance()->GetCurrentSceneID(),
		CSimon::GetIntance()->GetSecondWeapon(),
		CSimon::GetIntance()->GetAmount2ndWeapon()
	);
	LoadMusic();
	CSound::GetInstance()->play("MusicMap", 1, 10000);
	CCamera::GetInstance()->SetDefaultCam();
}

void CLostScene::Unload()
{
	CSound::GetInstance()->UnLoadSound("MusicMap");
}

void CLostScene::Update(DWORD dt)
{
	if (isEnter)
	{
		if (GetTickCount() - timeSelected >= TIME_SELECTED)
		{
			CSound::GetInstance()->stop("SELECTED");

			if (isContinue)
			{
				CSceneManager::GetInstance()->SetCurrentSceneID(1);
			}
			else
			{
				CSceneManager::GetInstance()->SetCurrentSceneID(0);
			}
			Unload();
			CSimon::GetIntance()->SetFinish(0);
			CSimon::GetIntance()->ResetSimon();
			CSceneManager::GetInstance()->GetCurrentScene()->Load();
			CSceneManager::GetInstance()->GetCurrentScene()->Update(dt);
		}
	}
}

void CLostScene::Render()
{
	csb->Render();
	font->Draw("GAME OVER", WIN_POSITION.x, WIN_POSITION.y);
	font->Draw("CONTINUE", ENTER_POSITION.x, ENTER_POSITION.y);
	font->Draw("END", END_POSITION.x, END_POSITION.y);
	if (isContinue)
		heartSel->Draw(CONTINUE_HEART_POSITION.x, CONTINUE_HEART_POSITION.y);
	else
		heartSel->Draw(END_HEART_POSITION.x, END_HEART_POSITION.y);
}

void CLostScene::Enter()
{
	isEnter = 1;
	CSound::GetInstance()->play("SELECTED", 1);
}

CLostScene::~CLostScene()
{

}

void CLostScenceKeyHandler::KeyState(BYTE* states)
{

}

void CLostScenceKeyHandler::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_UP:case DIK_DOWN:
		static_cast<CLostScene*>(CSceneManager::GetInstance()->GetCurrentScene())->isContinue = !static_cast<CLostScene*>(CSceneManager::GetInstance()->GetCurrentScene())->isContinue;
		break;
	case DIK_RETURN:
		if (static_cast<CLostScene*>(CSceneManager::GetInstance()->GetCurrentScene())->isEnter == false)
		{
			static_cast<CLostScene*>(CSceneManager::GetInstance()->GetCurrentScene())->Enter();
			static_cast<CLostScene*>(CSceneManager::GetInstance()->GetCurrentScene())->timeSelected = GetTickCount();
		}
		break;
	}
}

void CLostScenceKeyHandler::OnKeyUp(int KeyCode)
{

}
