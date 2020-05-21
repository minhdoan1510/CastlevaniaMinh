#include "Map.h"
#include<fstream>
#include "strsafe.h"

CMap::CMap(int _id,string filepath)
{
	id = _id;
	ReadFileMap(filepath);
}

CMap::~CMap()
{
}

void CMap::ReadFileMap(string filePath)
{
	ifstream f(filePath, ios::in);
	f >> rowMap >> columnMap >> _rowTile >> _columnTile >> floor >> sizeFrame >> _idTileTransparent;
	for (int i = 0; i < rowMap; i++)
		for (int j = 0; j < columnMap; j++)
		{
			f>>tileMap[i][j];
		}
	f.close();
	if (_idTileTransparent > (_rowTile * _columnTile - 1))
		_idTileTransparent = -1;
}

void CMap::DrawMap()
{
	CCamera* camera = CCamera::GetInstance();
	
	camera->SetBoundSize(columnMap * sizeFrame, rowMap * sizeFrame);
	
	RECT rectCam;
	rectCam.left = camera->GetXCam();
	rectCam.top = camera->GetYCam();
	rectCam.right = rectCam.left + camera->GetWidth();
	rectCam.bottom = rectCam.right + camera->GetHeight();

	RECT rectTileMapforCam;
	rectTileMapforCam.left = (int)(rectCam.left / sizeFrame);
	rectTileMapforCam.top = (int)(rectCam.top / sizeFrame);
	rectTileMapforCam.right = (1 + (int)(rectCam.right / sizeFrame));
	rectTileMapforCam.bottom = (1 + (int)(rectCam.bottom / sizeFrame));


	RECT rect;
	int frame;
	int m, n;

	for (int i = rectTileMapforCam.left; i <= rectTileMapforCam.right; i++)
	{
		for (int j = rectTileMapforCam.top; j <= rectTileMapforCam.bottom; j++)
		{
			if (i < 0 || j < 0)
				continue;
			if (i >= columnMap || j >= rowMap)
				continue;
			D3DXVECTOR3 pos(i * sizeFrame,j * sizeFrame,0);
			frame = tileMap[j][i];			
			m = frame % _columnTile;
			n = frame / _columnTile;
			rect.left = sizeFrame * m;
			rect.top = sizeFrame * n;
			rect.right = rect.left + sizeFrame;
			rect.bottom = rect.top + sizeFrame;

			sprite->DrawFrame(pos.x, pos.y, rect);
		}
	}

}

void CMap::DrawMapTransform(float xTrans, float yTrans, int directTrans)
{
	CCamera* camera = CCamera::GetInstance();

	//camera->SetBoundSize(columnMap * sizeFrame, rowMap * sizeFrame);
	
	RECT rectCam;
	rectCam.left = camera->GetXCam() + xTrans;
	rectCam.top = camera->GetYCam() + yTrans;
	rectCam.right = rectCam.left + camera->GetWidth();
	rectCam.bottom = rectCam.right + camera->GetHeight();

	RECT rectTileMapforCam;
	rectTileMapforCam.left = (int)(rectCam.left / sizeFrame);
	rectTileMapforCam.top = (int)(rectCam.top / sizeFrame);
	rectTileMapforCam.right = (1 + (int)(rectCam.right / sizeFrame));
	rectTileMapforCam.bottom = (1 + (int)(rectCam.bottom / sizeFrame));


	RECT rect;
	int frame;
	int m, n;

	for (int i = rectTileMapforCam.left; i <= rectTileMapforCam.right; i++)
	{
		for (int j = rectTileMapforCam.top+(rectTileMapforCam.top!=0); j <= rectTileMapforCam.bottom; j++)
		{
			if (i < 0 || j < 0)
				continue;
			if (i >= columnMap || j >= rowMap)
				continue;
			D3DXVECTOR3 pos(i * sizeFrame, j * sizeFrame, 0);
			frame = tileMap[j][i];
			m = frame % _columnTile;
			n = frame / _columnTile;
			rect.left = sizeFrame * m;
			rect.top = sizeFrame * n;
			rect.right = rect.left + sizeFrame;
			rect.bottom = rect.top + sizeFrame;

			sprite->DrawFrame(pos.x -xTrans, pos.y - yTrans, rect);
		}
	}
}

void CMap::DrawIntroScene() {
	RECT rect;
	int frame;
	int m, n;

	for (int i = 0; i < columnMap; i++)
	{
		for (int j = 0; j < rowMap; j++)
		{
			if (i < 0 || j < 0)
				continue;
			D3DXVECTOR3 pos(i * sizeFrame, j * sizeFrame, 0);
			frame = tileMap[j][i];
			m = frame % _columnTile;
			n = frame / _columnTile;
			rect.left = sizeFrame * m;
			rect.top = sizeFrame * n;
			rect.right = rect.left + sizeFrame;
			rect.bottom = rect.top + sizeFrame;
			pos.y -= PULL_SCREEN_Y;
			sprite->DrawFrame(pos.x, pos.y, rect);
		}
	}

}

void CMap::LoadMap(string fileTileMap)
{
	texture = CTextureManager::GetInstance()->Get(static_cast<ObjectType>(id + MAP));
	sprite = CSprites::GetInstance()->Get(id + MAP);
}

int CMap::GetMapWidth()
{
	return columnMap * sizeFrame;
}

int CMap::GetMapHeight()
{
	return rowMap * sizeFrame;
}

int CMap::GetFloorMap()
{
	return floor;
}

void CMap::SetSpriteTex(CSprite* s,	LPDIRECT3DTEXTURE9 t)
{
	sprite = s;
	texture = t;
}

void CMap::SetBoundaryLeftRight(int floor)
{
	float boundL, boundR;
	if (_idTileTransparent != -1)
	{
		int j_tile = (floor - 1) * (rowMap / this->floor);
		for (int i = 0; i < columnMap; i++)//check boundleft
		{
			if (tileMap[j_tile][i] != _idTileTransparent)
			{
				boundL = i * sizeFrame;
				break;
			}
		}

		for (int i = columnMap-1; i >= 0; i--)//check boundright
		{
			if (tileMap[j_tile][i] != _idTileTransparent)
			{
				boundR = (i+1) * sizeFrame;
				break;
			}
		}
	}
	else
	{
		boundL = 0;
		boundR = GetMapWidth();
	}
	if (boundL >= boundR)
	{
		DebugOut(L"[Map.cpp] Error bound map!!!");
		return;
	}
	CCamera::GetInstance()->SetBoundLeftRight(boundL, boundR);
}