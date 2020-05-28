#include <iostream>
#include <fstream>
#include <string>
#include "PlayScence.h"
#include "Utils.h"
#include "TextureManager.h"
#include "Sprites.h"
#include "Camera.h"
#include "Trigger.h"
#include "Effect.h"
#include "strsafe.h"
#include "EnemyDoor.h"
#include "MoneyEffect.h"
#include "DeadEffect.h"
#include "BrickEffect.h"

using namespace std;

CPlayScene::CPlayScene(int id, string pathFolder) :CScene(id, pathFolder)
{
	key_handler = new CPlayScenceKeyHandler(this);
	filePathObj = "Resources/map/" + pathFolder + "/obj.txt";
	filePathMap = "Resources/map/" + pathFolder + "/map.txt";
	timeGame = TIME_DEFAULT_SCENE;
}

void CPlayScene::LoadMap()
{
	map = CMapManager::GetIntance()->Get(id);
	if (map == NULL)
	{
		map = new CMap(id, filePathMap);
		CMapManager::GetIntance()->Add(id, map);
	}

	map->LoadMap("Resources /map/"+folderPath+"/map.png");
}

void CPlayScene::LoadObject()
{
	grid = new CGrid(map->GetMapWidth(), map->GetMapHeight(), filePathObj);
	grid->LoadGrid();
}

void CPlayScene::Load()
{
	CTextureManager::GetInstance()->LoadResourceScene("Resources/map/" + folderPath + "/texture.txt");
	CSprites::GetInstance()->LoadResourceScene("Resources/map/" + folderPath + "/sprite.txt");
	CAnimationSets::GetInstance()->LoadResourceScene("Resources/map/" + folderPath);

	CSimon::GetIntance()->SetAnimationSet(CAnimationSets::GetInstance()->Get(SIMON));
	LoadMap();
	LoadObject();
	lifeTime = GetTickCount64();
	cScoreBoard = new CScoreBoard();
	isTransScene = 0;
}

void CPlayScene::Update(DWORD dt)
{
	DWORD now = GetTickCount64();
	if (now - lifeTime > 1000)
	{
		lifeTime = now;
		timeGame--;
	}
	if (timeGame <= 0)
	{
		GameOver();
	}
	
	float _x, _y;
	CSimon::GetIntance()->GetPosition(_x, _y);

	//SetCamera
	map->SetBoundaryLeftRight(1 + (int)(_y + SIMON_BBOX_HEIGHT / 2) / (CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap()));

	CCamera::GetInstance()->SetPosition(
		(int)(_x - SCREEN_WIDTH / 2 + SIMON_BBOX_WIDTH / 2),
		(CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap()) *
		((int)(_y + SIMON_BBOX_HEIGHT / 2) / (CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap())),
		(CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap()));
	//

	vector<LPGAMEOBJECT> coObjects = grid->GetListObj();

	CSimon::GetIntance()->Update(dt, &coObjects);//Update CSimon

	if (CSimon::GetIntance()->IsFreeze()) return;

	if (CSceneManager::GetInstance()->IsPassScene())
	{
		//CCamera::GetInstance()->AutoCamX(SCREEN_WIDTH / 2, CSimon::GetIntance()->GetNx());
		CSceneManager::GetInstance()->Update(dt);
		//CSceneManager::GetInstance()->EndPassScene();
		return;
	}

	if (_x + SIMON_BBOX_WIDTH > map->GetMapWidth())
		CSimon::GetIntance()->SetPosition(map->GetMapWidth() - SIMON_BBOX_WIDTH, _y);

	/*map->SetBoundaryLeftRight(1 + (int)(_y + SIMON_BBOX_HEIGHT / 2) / (CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap()));

	CCamera::GetInstance()->SetPosition(
		(int)(_x - SCREEN_WIDTH / 2 + SIMON_BBOX_WIDTH / 2),
		(CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap()) *
		((int)(_y + SIMON_BBOX_HEIGHT / 2) / (CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap())),
		(CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap()));*/


	// Kiểm tra đã pass map hay chưa
	

	//Kiểm tra có obj nào cần được thêm vào hay không
	vector<CGameObject*> addAfterUpdate= CSimon::GetIntance()->GetAbjAddAfterUpdate();
	for (int i = 0; i < addAfterUpdate.size(); i++)
	{
		grid->InsertGrid(addAfterUpdate.at(i));
	}
	coObjects = grid->GetListObj();

	//update obj
	for (int i = 0; i < coObjects.size(); i++)
	{
		coObjects[i]->Update(dt, &coObjects);
	}

	//Kiểm tra có tồn tại enemy nào cần thêm vào sau khi update object
	vector<CGameObject*>* enemySpawn = new vector<CGameObject*>();
	for (int i = 0; i < coObjects.size(); i++)
	{
		if (dynamic_cast<CEnemyDoor*>(coObjects[i]))
		{
			enemySpawn = dynamic_cast<CEnemyDoor*>(coObjects[i])->GetEnemySpawn();
			for (int i = 0; i < enemySpawn->size(); i++)
			{
				grid->InsertGrid(enemySpawn->at(i));
			}
			enemySpawn->clear();
		}
	}

	//update effect
	for (int i = 0; i < effects.size() ; i++)
	{
		if (effects[i]==NULL) continue;
		effects[i]->Update(dt);
		if (effects[i]->IsFinish())
		{
			if (effects[i]->GetItemHolder() != Null)
			{
				float _x, _y;
				effects[i]->GetPosition(_x, _y);
				LPGAMEOBJECT _item = new CItem(_x, _y, effects[i]->GetItemHolder());
				grid->InsertGrid(_item);
			}
			effects.erase(effects.begin() + i);
		
		}
	}

	//Remove and add when destroy
	if (CSimon::GetIntance()->GetDeath() == true) {
		if (CSimon::GetIntance()->GetLifeSimon() > 0)
		{
			CSceneManager::GetInstance()->ReloadScene();
		}
		else
		{
			CSceneManager::GetInstance()->LostGame();
			return;
		}
		CSimon::GetIntance()->SetFinish(0);
		CSceneManager::GetInstance()->GetCurrentScene()->Update(dt);
		return;
	}
	int k = 0;
	for (int i = 0; i < coObjects.size() - k; i++)
	{
		if ((coObjects.at(i)->GetDeath() == true && coObjects.at(i)->GetObjType() != ITEM))
		{
			float _x, _y;
			coObjects.at(i)->GetPosition(_x, _y);
			//grid->InsertGrid(_item); 
			CGameObject* backup = coObjects.at(i);
			
			coObjects.erase(coObjects.begin() + i);
					
			float _xtemp, _ytemp;
			backup->GetPosition(_xtemp, _ytemp);
			switch (backup->GetObjType())
			{
			case BRICK:
				effects.push_back(new CBrickEffect(_xtemp, _ytemp, BRICK_EFFECT, Null));
				if (backup->GetItemHolder() != Null)
				{
					LPGAMEOBJECT _item = new CItem(_x, _y, backup->GetItemHolder());
					grid->InsertGrid(_item);
				}
				break;
			default:
				effects.push_back(new CDeadEffect(_xtemp, _ytemp, DEATH_ANI, 200, backup->GetItemHolder()));
				break;
			}

			grid->RemoveObj(backup);
			k++;
		}
		else if (coObjects.at(i)->GetDeath() == true && coObjects.at(i)->GetObjType() == ITEM)
		{
			float _xtemp, _ytemp;
			coObjects.at(i)->GetPosition(_xtemp, _ytemp);

			CEffect* tempEffect;

			switch (static_cast<CItem*> (coObjects.at(i))->GetItemType())
			{
			case MONEY_RED_ITEM: // 100
				tempEffect = new CMoneyEffect(_xtemp, _ytemp, MONEY_100_EFFECT);
				CSceneManager::GetInstance()->ScoreGame += 100;
				break;
			case MONEY_WHITE_ITEM: //700
				tempEffect = new CMoneyEffect(_xtemp, _ytemp, MONEY_700_EFFECT);
				CSceneManager::GetInstance()->ScoreGame += 700;
				break;
			case MONEY_BOX_YELLOW_ITEM: //700
				tempEffect = new CMoneyEffect(_xtemp, _ytemp, MONEY_700_EFFECT);
				CSceneManager::GetInstance()->ScoreGame += 700;
				break;
			case CROWM_YELLOW_ITEM: //2000
				tempEffect = new CMoneyEffect(_xtemp, _ytemp, MONEY_700_EFFECT);
				CSceneManager::GetInstance()->ScoreGame += 2000;
			default:
				tempEffect = nullptr;
				break;
			}
			CGameObject* backup = coObjects.at(i);
			for (int m = 0; m < coObjects.size(); m++)
			{
				if (coObjects.at(m) == coObjects.at(i))
				{
					coObjects.erase(coObjects.begin() + m);
					break;
				}
			}
			grid->RemoveObj(backup);
			k++;
			effects.push_back(tempEffect);
		}
	}
	grid->UpdateGrid(coObjects);
	//Update ScoreBoard
	cScoreBoard->Update(timeGame, CSceneManager::GetInstance()->ScoreGame, CSimon::GetIntance()->GetHeart(), CSimon::GetIntance()->GetLifeSimon(), CSimon::GetIntance()->GetHPSimon(), 16, CSceneManager::GetInstance()->GetCurrentSceneID(), CSimon::GetIntance()->GetSecondWeapon(), CSimon::GetIntance()->GetAmount2ndWeapon());
}

void CPlayScene::Render()
{
	map->DrawMap();
	vector<LPGAMEOBJECT> coObjects = grid->GetListObj();
	for (int i = 0; i < coObjects.size(); i++)
		coObjects[i]->Render();

	for (int i = 0; i < effects.size(); i++)
	{
		if (effects[i] == NULL) continue;
		effects[i]->Render();
	}

	CSimon::GetIntance()->Render();
	cScoreBoard->Render();
}

void CPlayScene::Unload()
{
	//SAFE_DELETE(map);
	grid->UnLoadGrid();
	CTextureManager::GetInstance()->ClearTextureScene();
	CSprites::GetInstance()->ClearSpritesScene();
	CAnimations::GetInstance()->ClearAniScene();
	CAnimationSets::GetInstance()->ClearAniScene();
}

void CPlayScene::GameOver()
{
	CSimon::GetIntance()->DeathSimon();
}

CMap* CPlayScene::GetMap()
{
	return map;
}

bool CPlayScene::IsTransScene()
{
	return isTransScene;
}

D3DXVECTOR2 CPlayScene::GetPosSimonDefault()
{
	return grid->GetPosSimonDefault();
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CSimon *simon= CSimon::GetIntance();
	if (simon->IsAutoGo()|| simon->IsInjured()||simon->IsDeading()) return;
	switch (KeyCode)
	{
	case DIK_S:
		if (simon->IsOnStair())
			break;
		simon->Jump();
		break;
	case DIK_A: 
		if (game->IsKeyDown(DIK_UP))
				simon->Attack(0);
		else
			simon->Attack(1);
		simon->Stop();
		break;
	case DIK_P:
		CSceneManager::GetInstance()->StartPassScene(0,0,1);
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CSimon* simon = CSimon::GetIntance();
	if (simon->IsAutoGo()||simon->IsProcessStair()|| simon->IsInjured()|| simon->IsDeading()) return;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (simon->IsOnStair())
		{
			if (simon->GetNx() == 1)
				simon->Go();
			
			simon->Right();
		}
		else
		{
			if (game->IsKeyDown(DIK_DOWN))
			{
				simon->Sit();
			}
			else {
				simon->Go();
				simon->ResetSit();
			}

			simon->Right();
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->IsOnStair())
		{
			if (simon->GetNx() == -1)
				simon->Go();

			simon->Left();
		}
		else
		{

			if (game->IsKeyDown(DIK_DOWN))
				simon->Sit();
			else {
				simon->Go();
				simon->ResetSit();
			}
			simon->Left();
		}
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->IsOnStair())
		{
			//DebugOut(L"temp");
			if (-1 * simon->GetNx() == simon->GetDirectStair())
				simon->Go();
			if (simon->GetDirectStair() == 1)
			{
				simon->Left();
			}
			else
			{
				simon->Right();
			}
		}
		else
		{
			simon->Sit();
		}
	}
	else if (game->IsKeyDown(DIK_UP)) {
		if (simon->IsOnStair())
		{
			if (simon->GetNx() == simon->GetDirectStair())
				simon->Go();
			if (simon->GetDirectStair() == 1)
			{
				simon->Right();
			}
			else
			{
				simon->Left();
			}
		}
		else
		{
			simon->WantUpOnStair();
			simon->Stop();
			//simon->Go();
		}
	}
	else
	{
		simon->Stop();
	}
}