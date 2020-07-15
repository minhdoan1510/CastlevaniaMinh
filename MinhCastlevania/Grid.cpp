#include <fstream>
#include <algorithm>
#include "Grid.h"
#include "Simon.h"
#include "Brick.h"
#include "Candle.h"
#include "Trigger.h"
#include "Item.h"
#include "SceneManager.h"
#include "Enemy.h"
#include "BlackKnight.h"
#include "Bat.h"
#include "Bridge.h"
#include "Raven.h"
#include "Ghost.h"
#include "Feaman.h"
#include "WhiteSkeleton.h"
#include "Zombie.h"
#include "EnemyDoor.h"
#include "PhantomBat.h"

vector<CGameObject*> CGrid::FilterObjectDuplicate(vector<CGameObject*> objs)
{
	std::sort(objs.begin(), objs.end());
	objs.erase(std::unique(objs.begin(), objs.end()), objs.end());
	return objs;
}

CGrid::CGrid(int _mapW, int _mapH, string _fileobj)
{
	cellH = CELL_SIZE.y;
	cellW = CELL_SIZE.x;
	columnGrid = 1 + _mapW / cellW;
	rowGrid = 1 + _mapH / cellH;
	fileobj = _fileobj;
}


void CGrid::LoadGrid()
{
	ifstream ifs(fileobj);
	int type;
	float _x, _y, _w, _h;
	int _type, _item, directStair, _direct;
	int _xGrid, _yGrid;
	int _countGrid;
	vector<pair<int, int>> posGrid;
	CGameObject* a;
	while (true)
	{
		posGrid.clear();
		ifs >> type;
		if (type == -555) break;
		ifs >>_countGrid;

		for (int i = 0; i < _countGrid; i++)
		{
			ifs >> _xGrid >> _yGrid;
			posGrid.push_back(make_pair(_xGrid, _yGrid));
		}
		switch (static_cast<ObjectType>(type))
		{
		case SIMON:
			ifs >> _x >> _y;
			//CSimon::GetIntance()->SetPosition(_x, _y);
			if (CSceneManager::GetInstance()->GetCurrentSceneID() == 1)
			{
				CSimon::GetIntance()->SetPosition(_x, _y);
			}
			posSimonDefault = D3DXVECTOR2(_x, _y);
			a = CSimon::GetIntance();
			break;
		case BRICK:
			ifs >> _x >> _y >> _w >> _h >> _type;
			if (_type != BRICK_MODEL_TRANSPARENT_1 && _type != BRICK_MODEL_TRANSPARENT_2)
			{
				ifs >> _item;
				a = new CBrick(_x, _y, _w, _h, _type, static_cast <ObjectType>(_item));
			}
			else
				a = new CBrick(_x, _y, _w, _h, _type);
			break;
		case BRIDGE:
			ifs >> _x >> _y;
			a = new CBridge(_x, _y);
			break;
		case CANDLE:
			ifs >> _x >> _y >> _item >> _type;
			a = new CCandle(_x, _y, static_cast <ObjectType>(_item), static_cast <ObjectType>(_type));
			break;
		case TRIGGER:
			ifs >> _x >> _y >> _w >> _h >> _type >> directStair;
			a = new CTrigger(_x, _y, _w, _h, static_cast <ObjectType>(_type), directStair);
			if (_type == ITEM_HIDDEN_TRIGGER)
			{
				ifs >> _x >> _y >> _item;
				static_cast<CTrigger*>(a)->SetItemHolder(new CItem(_x, _y, static_cast <ObjectType>(_item)));
			}
			break;
		case ENEMY:
			ifs >> _x >> _y >> _type;
			switch (static_cast<ObjectType>(_type))
			{
			case BLACKKNIGHT:
				a = new CBlackKnight(_x, _y);
				break;
			case BAT:
				a = new CBat(_x, _y);
				break;
			case GHOST:
				a = new CGhost(_x, _y);
				break;
			case RAVEN:
				a = new CRaven(_x, _y);
				break;
			case FEAMAN:
				a = new CFeaman(_x, _y);
				break;
			case WHITESKELETON:
				a = new CWhiteSkeleton(_x, _y);
				break;
			case ZOMBIE:
				a = new CZombie(_x, _y);
				break;
			case PHANTOMBAT:
				a = new CPhantomBat(_x, _y);
				break;
			default:
				a = nullptr;
				break;
			}
			break;

		case ENEMY_DOOR:
			ifs >> _x >> _y >> _type >> _direct;
			a = new CEnemyDoor(_x, _y, static_cast<ObjectType>(_type), _direct);
			break;
		default:
			a = nullptr;
			break;
		}
		if (static_cast<ObjectType>(type) != SIMON && a != nullptr)
			InsertGrid(a, posGrid);
	}
	ifs.close();
}

void CGrid::InsertGrid(CGameObject* obj, vector<pair<int, int>> posGrid)
{
	for (int i = 0; i < posGrid.size(); i++)
	{
		cells[posGrid.at(i).first][posGrid.at(i).second].push_back(obj);
	}
}


/*void CGrid::LoadGrid()
{
	//cells = new vector<CGameObject*> * [rowGrid+1];
	//for (int i = 0; i < rowGrid+1; i++)
	//{
		//cells[i] = new vector<CGameObject*>[columnGrid+1];
	//}
	ifstream ifs(fileobj);
	int type;
	float _x, _y, _w, _h;
	int _type, _item, directStair, _direct;
	CGameObject* a;
	while (true)
	{
		ifs >> type;
		if (type == -555) break;
		switch (static_cast<ObjectType>(type))
		{
		case SIMON:
			ifs >> _x >> _y;
			//CSimon::GetIntance()->SetPosition(_x, _y);
			if (CSceneManager::GetInstance()->GetCurrentSceneID() == 1)
			{
				CSimon::GetIntance()->SetPosition(_x, _y);
			}
			posSimonDefault = D3DXVECTOR2(_x, _y);
			a = CSimon::GetIntance();
			break;
		case BRICK:
			ifs >> _x >> _y >> _w >> _h >> _type;
			if (_type != BRICK_MODEL_TRANSPARENT_1 && _type != BRICK_MODEL_TRANSPARENT_2)
			{
				ifs >> _item;
				a = new CBrick(_x, _y, _w, _h, _type, static_cast <ObjectType>(_item));
			}
			else
			a = new CBrick(_x, _y, _w, _h, _type);
			break;
		case BRIDGE:
			ifs >> _x >> _y ;
			a = new CBridge(_x, _y);
			break;
		case CANDLE:
			ifs >> _x >> _y >> _item >> _type;
			a = new CCandle(_x, _y, static_cast <ObjectType>(_item), static_cast <ObjectType>(_type));
			break;
		case TRIGGER:
			ifs >> _x >> _y >> _w >> _h >> _type >> directStair;
			a = new CTrigger(_x, _y, _w, _h, static_cast <ObjectType>(_type), directStair);
			if (_type == ITEM_HIDDEN_TRIGGER)
			{
				ifs >> _x >> _y >>_item;
				static_cast<CTrigger*>(a)->SetItemHolder(new CItem(_x, _y, static_cast <ObjectType>(_item)));
			}
			break;
		case ENEMY:
			ifs >> _x >> _y >> _type;
			switch (static_cast<ObjectType>(_type))
			{
			case BLACKKNIGHT:
				a = new CBlackKnight(_x, _y);
				break;
			case BAT:
				a = new CBat(_x, _y);
				break;
			case GHOST:
				a = new CGhost(_x, _y);
				break;
			case RAVEN:
				a = new CRaven(_x, _y);
				break; 
			case FEAMAN:
				a = new CFeaman(_x, _y);
				break;
			case WHITESKELETON:
				a = new CWhiteSkeleton(_x, _y);
				break;
			case ZOMBIE:
				a = new CZombie(_x, _y);
				break;
			case PHANTOMBAT:
				a = new CPhantomBat(_x, _y);
				break;
			default:
				a = nullptr;
				break;
			}
			break;

		case ENEMY_DOOR:
			ifs >> _x >> _y >> _type >>_direct;
			a = new CEnemyDoor(_x, _y, static_cast<ObjectType>(_type), _direct);
			break;
		default:
			a = nullptr;
			break;
		}
		if (static_cast<ObjectType>(type) != SIMON && a != nullptr)
			InsertGrid(a);
	}
	ifs.close();
}*/

CGrid::~CGrid()
{
}

void CGrid::UpdateGrid(vector<CGameObject*> objects)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CBrick*>(objects.at(i)))
		{
			RemoveObj(objects.at(i),0);
			InsertGrid(objects.at(i));
		}
	}
}

void CGrid::RemoveObj(CGameObject* obj,bool isDeletePointer)
{
	RECT rectObj = obj->GetBBox();
	int minRow = int(rectObj.top) / cellH-3;
	int maxRow = int(rectObj.bottom) / cellH+3;
	int minColumn = int(rectObj.left) / cellW-3;
	int maxColumn = int(rectObj.right) / cellW+3;

	if (minRow < 0)
		minRow = 0;
	if (maxRow >= rowGrid)
		maxRow = rowGrid -1;
	if (minColumn < 0)
		minColumn = 0;
	if (maxColumn >= columnGrid)
		maxColumn = columnGrid-1;

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minColumn; j <= maxColumn; j++)
		{
			for (int m = 0; m < cells[i][j].size(); m++)
			{
				if (cells[i][j].at(m) == obj)
				{
					cells[i][j].erase(cells[i][j].begin() + m);
				}
			}
		}
	}
	if(isDeletePointer)
		SAFE_DELETE(obj);
}

void CGrid::UnLoadGrid()
{
	/*for (int i = 0; i < rowGrid; i++)
	{
		delete[] cells[i];
	}

	delete[] cells;*/
}

void CGrid::InsertGrid(CGameObject* obj)
{
	RECT rectObj = obj->GetBBox();
	int minRow = int(rectObj.top) / cellH;
	int maxRow = int(rectObj.bottom) / cellH;
	int minColumn = int(rectObj.left) / cellW;
	int maxColumn = int(rectObj.right) / cellW;

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minColumn; j <= maxColumn; j++)
		{
			cells[i][j].push_back(obj);
		}
	}
}

D3DXVECTOR2 CGrid::GetPosSimonDefault()
{
	return posSimonDefault;
}

vector<CGameObject*> CGrid::GetListObj()
{
	RECT rectCam = CCamera::GetInstance()->GetRectCam();
	int minRow = int(rectCam.top) / cellH;
	int maxRow = int(rectCam.bottom) / cellH; // sử dụng floor

	//int minRow = (CCamera::GetInstance()->GetCurrentFloor()-1)*2;
	//int maxRow = minRow + 2; // sử dụng floor

	int minColumn = int(rectCam.left) / cellW;
	int maxColumn = int(rectCam.right) / cellW;

	vector<CGameObject*> result;

	for (int i = minRow; i <= maxRow; i++)
	{
		for (int j = minColumn; j <= maxColumn; j++)
		{
			for (int m = 0; m < cells[i][j].size(); m++)
				result.push_back(cells[i][j].at(m));
		}
	}
	result = FilterObjectDuplicate(result);
	
	return result;
}