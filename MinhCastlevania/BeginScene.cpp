#include "BeginScene.h"
#include "Animations.h"
#include "SceneManager.h"
#include "Simon.h"
#include "Brick.h"
#include "CloudIntro.h"
#include "BatIntro.h"
#include <fstream>

CBeginScene::CBeginScene(int id, string folderPath) :CScene(id, folderPath)
{
	key_handler = new CBeginScenceKeyHandler(this);
}

void CBeginScene::LoadObjectIntroScene()
{

	objects->clear();
	ifstream ifs("Resources/IntroScene/BeginScene/obj.txt");
	int type;
	float _x, _y, _w, _h;
	int _type, _item, directStair, _nx;
	CGameObject* a;
	while (true)
	{
		ifs >> type;
		if (type == -555) break;
		switch (static_cast<ObjectType>(type))
		{
		case SIMON:
			ifs >> _x >> _y;
			CSimon::GetIntance()->SetPosition(_x, _y);
			a = CSimon::GetIntance();
			break;
		case BRICK:
			ifs >> _x >> _y >> _w >> _h >> _type;
			a = new CBrick(_x, _y, _w, _h, _type);
			break; 
		case BAT_INTRO:
				ifs >> _x >> _y >> _nx;
				a = new CBatIntro(_x, _y,_nx);
				break;
		case CLOUD_INTRO:
			ifs >> _x >> _y >> _nx;
			a = new CCloudIntro(_x, _y, _nx);
			break;
		default:
			a = nullptr;
			break;
		}
		if (static_cast<ObjectType>(type) != SIMON && a != nullptr)
			objects->push_back(a);
	}
	ifs.close();
}

void CBeginScene::Load()
{
	CTextureManager::GetInstance()->LoadResourceScene("Resources/IntroScene/BeginScene/texture.txt");
	CSprites::GetInstance()->LoadResourceScene("Resources/IntroScene/BeginScene/sprite.txt");
	CAnimationSets::GetInstance()->LoadResourceScene("Resources/IntroScene/BeginScene");
	mapIntro = new CMap(0, "Resources/IntroScene/BeginScene/tile.txt");
	mapIntro->SetSpriteTex(CSprites::GetInstance()->Get(0), CTextureManager::GetInstance()->Get(static_cast<ObjectType>(0)));
	mapIntro->SetBoundaryLeftRight(1);
	font = new CFont();
	isJoinGame = 0;
	CCamera::GetInstance()->SetPosition(0, 0, 1);
	lTime = 0;
	aniB = CAnimations::GetInstance()->Get(ANI_BEGIN_SCENE);
	isIntro = 1;
	objects = new vector<CGameObject*>();
	CSimon::GetIntance()->SetAnimationSet(CAnimationSets::GetInstance()->Get(SIMON));
}

void CBeginScene::Unload()
{
	SAFE_DELETE(mapIntro);
	CTextureManager::GetInstance()->ClearTextureScene();
	CSprites::GetInstance()->ClearSpritesScene();
	CAnimations::GetInstance()->ClearAniScene();
	CAnimationSets::GetInstance()->ClearAniScene();
}

void CBeginScene::Update(DWORD dt)
{
	if (isIntro)
	{
		if (isJoinGame && lTime == 0)
		{
			lTime = GetTickCount();
		}
		if (lTime != 0 && GetTickCount() - lTime >= JOIN_GAME_TIME)
		{
			isIntro = 0;
			SAFE_DELETE(mapIntro);
			mapIntro = new CMap(0, "Resources/IntroScene/BeginScene/map.txt");
			mapIntro->SetSpriteTex(CSprites::GetInstance()->Get(200), CTextureManager::GetInstance()->Get(static_cast<ObjectType>(200)));
			mapIntro->SetBoundaryLeftRight(1);
			LoadObjectIntroScene();
			CSimon::GetIntance()->SetAutoGo(DISTANCE_SIMON_AUTO, -1, 1);
			lTime = 0;
			cScoreBoard = new CScoreBoard();
		}
	}
	if(!isIntro)
	{
		for (int i = 0; i < objects->size(); i++)
			objects->at(i)->Update(dt);
		CSimon::GetIntance()->Update(dt,objects);
		if (!CSimon::GetIntance()->IsAutoGo())
		{
			if (lTime != 0 && GetTickCount() - lTime >= SIMON_ENTER_GAME_TIME)
			{
				CSimon::GetIntance()->EnterIntroGameState(0);
				CSceneManager::GetInstance()->PassScene();
				CSceneManager::GetInstance()->Update(dt);
				return;
			}
			else if (lTime == 0)
			{
				lTime = GetTickCount();
				CSimon::GetIntance()->EnterIntroGameState(1);
			}
		}
		cScoreBoard->Update(300, CSceneManager::GetInstance()->ScoreGame, CSimon::GetIntance()->GetHeart(), CSimon::GetIntance()->GetLifeSimon(), CSimon::GetIntance()->GetHPSimon(), 16, CSceneManager::GetInstance()->GetCurrentSceneID(), CSimon::GetIntance()->GetSecondWeapon(), CSimon::GetIntance()->GetAmount2ndWeapon());
	}
}

void CBeginScene::Render()
{
	if (isIntro)
	{
		mapIntro->DrawIntroScene();
		if (isJoinGame)
		{
			if (((GetTickCount() - lTime) / FONT_FRAME_TIME) % 2 == 0)
			{
				font->Draw("PUSH START KEY", JOIN_GAME_POSITION.x, JOIN_GAME_POSITION.y);
			}
		}
		else
			font->Draw("PUSH START KEY", JOIN_GAME_POSITION.x, JOIN_GAME_POSITION.y);
		aniB->Render(ANI_GAME_POSITION.x, ANI_GAME_POSITION.y, 0);
	}
	else
	{
		mapIntro->DrawMap();
		for (int i = 0; i < objects->size(); i++)
			objects->at(i)->Render();
		CSimon::GetIntance()->Render();
		cScoreBoard->Render();
	}
}

CBeginScene::~CBeginScene()
{
}

void CBeginScenceKeyHandler::KeyState(BYTE* states)
{
}

void CBeginScenceKeyHandler::OnKeyDown(int KeyCode)
{
	if (!static_cast<CBeginScene*>(CSceneManager::GetInstance()->GetCurrentScene())->isJoinGame)
		static_cast<CBeginScene*>(CSceneManager::GetInstance()->GetCurrentScene())->isJoinGame = 1;
}

void CBeginScenceKeyHandler::OnKeyUp(int KeyCode)
{
}
