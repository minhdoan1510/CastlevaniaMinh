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

using namespace std;

CPlayScene::CPlayScene(int id) :CScene(id)
{
	key_handler = new CPlayScenceKeyHandler(this);
	string path = "Resources/map/" + to_string(id) + "/obj.txt";
	filePathObj = ToLPCWSTR(path);
	path = "Resources/map/" + to_string(id) + "/map.txt";
	filePathMap = ToLPCWSTR(path);
	timeGame = 400;
	ScoreGame = 0;
}

void CPlayScene::LoadMap()
{
	map = CMapManager::GetIntance()->Get(id);
	if (map == NULL)
	{
		map = new CMap(id, filePathMap);
		CMapManager::GetIntance()->Add(id, map);
	}
	map->LoadMap();
}

void CPlayScene::LoadObject()
{
	for (auto item: objects)
	{
		SAFE_DELETE(item);
	}
	objects.clear();
	ifstream ifs(filePathObj);
	/*int _row;
	ifs >> _row;*/
	int type;
	float _x, _y, _w, _h;
	int _type, _item, directStair;
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
		case CANDLE:
			ifs >> _x >> _y >> _item >> _type;
			a = new CCandle(_x, _y, static_cast <ObjectType>(_item), static_cast <ObjectType>(_type));
			break;
		case TRIGGER:
			ifs >> _x >> _y >> _w >> _h >> _type >> directStair;
			a = new CTrigger(_x, _y, _w, _h, static_cast <ObjectType>(_type),directStair);
			break;
		default:
			break;
		}
		objects.push_back(a);
	}
	ifs.close();
}

void CPlayScene::Load()
{
	LoadMap();
	LoadObject();
	if (CTextureManager::GetInstance()->Get(BBOX) == NULL)
		CTextureManager::GetInstance()->Add(BBOX, "Resources/bbox.png");
	lifeTime = GetTickCount64();
	cScoreBoard = new CScoreBoard();
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

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}


	coObjects.at(0)->Update(dt, &coObjects);//Update CSimon
	// Kiểm tra đã pass map hay chưa
	if (CSceneManager::GetInstance()->IsPassScene())
	{
		CSceneManager::GetInstance()->GetCurrentScene()->Update(dt);
		return;
	}


	for (size_t i = 1; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	//Remove and add when destroy
	int k = 0;
	for (int i = 0; i < objects.size() - k; i++)
	{
		if ((objects.at(i)->GetDeath() == true && objects.at(i)->GetObjType() != ITEM))
		{
			if ((!dynamic_cast<CSimon*>(objects.at(i))))
			{
				float _x, _y;
				objects.at(i)->GetPosition(_x, _y);
				LPGAMEOBJECT _item = new CItem(_x, _y, objects.at(i)->GetItemHolder());
				objects.push_back(_item);
				delete objects.at(i);
				objects.erase(objects.begin() + i);
				k++;
			}
			else
			{
				if (CSimon::GetIntance()->GetLifeSimon() > 0)
				{
					CSceneManager::GetInstance()->ReloadScene();
				}
				else
				{
					CSceneManager::GetInstance()->LostGame();
				}
				CSimon::GetIntance()->SetFinish(0);
				CSceneManager::GetInstance()->GetCurrentScene()->Update(dt);
				return;
			}
		}
		else if (objects.at(i)->GetDeath() == true && objects.at(i)->GetObjType() == ITEM)
		{
			float _xtemp, _ytemp;
			objects.at(i)->GetPosition(_xtemp, _ytemp);

			switch (static_cast<CItem*> (objects.at(i))->GetItemType())
			{
			case MONEY_RED_ITEM: // 100
				objects.push_back(new CEffect(_xtemp, _ytemp, MONEY_100_EFFECT));
				ScoreGame += 100;
				break;
			case MONEY_WHITE_ITEM: //700
				objects.push_back(new CEffect(_xtemp, _ytemp, MONEY_700_EFFECT));
				ScoreGame += 700;
				break;
			case MONEY_BOX_YELLOW_ITEM: //700
				objects.push_back(new CEffect(_xtemp, _ytemp, MONEY_700_EFFECT));
				ScoreGame += 700;
				break;
			case CROWM_YELLOW_ITEM: //2000
				objects.push_back(new CEffect(_xtemp, _ytemp, MONEY_2000_EFFECT));
				ScoreGame += 700;
			default:
				break;
			}

			delete objects.at(i);
			objects.erase(objects.begin() + i);
			k++;
		}
	}

	float _x, _y;
	CSimon::GetIntance()->GetPosition(_x, _y);
	if (_x + SIMON_BBOX_WIDTH > map->GetMapWidth())
		CSimon::GetIntance()->SetPosition(map->GetMapWidth() - SIMON_BBOX_WIDTH, _y);

	map->SetBoundaryLeftRight(1+(int)(_y + SIMON_BBOX_HEIGHT / 2) / (CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap()));

	CCamera::GetInstance()->SetPosition(
		(int)(_x - SCREEN_WIDTH / 2 + SIMON_BBOX_WIDTH / 2),
		(CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap())* 
		((int)(_y + SIMON_BBOX_HEIGHT / 2) / (CMapManager::GetIntance()->Get(id)->GetMapHeight() / CMapManager::GetIntance()->Get(id)->GetFloorMap())));

	//Update ScoreBoard
	cScoreBoard->Update(timeGame, ScoreGame, CSimon::GetIntance()->GetHeart(), CSimon::GetIntance()->GetLifeSimon(), CSimon::GetIntance()->GetHPSimon(), 16, CSceneManager::GetInstance()->GetCurrentSceneID(), CSimon::GetIntance()->GetSecondWeapon());
}

void CPlayScene::Render()
{
	map->DrawMap();
	for (int i = 1; i < objects.size(); i++)
		objects[i]->Render();

	objects[0]->Render();
	cScoreBoard->Render();
}

void CPlayScene::Unload()
{
	SAFE_DELETE(map);
	for (int i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CSimon*>(objects.at(i)))
			SAFE_DELETE(objects[i]);
	}
	objects.clear();
}

void CPlayScene::GameOver()
{
	CSimon::GetIntance()->DeathSimon();
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	CSimon *simon= CSimon::GetIntance();
	if (simon->IsAutoGo()) return;
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
		CSceneManager::GetInstance()->PassScene();
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
	if (simon->IsAutoGo()||simon->IsProcessStair()) return;
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
			DebugOut(L"temp");
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
			simon->Go();
		}
	}
	else
	{
		simon->Stop();
	}
}