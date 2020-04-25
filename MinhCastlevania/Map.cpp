#include "Map.h"
#include<fstream>

CMap::CMap(int _id,LPCWSTR filepath)
{
	id = _id;
	ReadFileMap(filepath);
}

CMap::~CMap()
{
}

void CMap::ReadFileMap(LPCWSTR filePath)
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

void CMap::LoadMap()
{
	texture = CTextureManager::GetInstance()->Get(static_cast<ObjectType>(id + MAP));
	if (CSprites::GetInstance()->Get(id + MAP1) == NULL)
	{
		CSprites::GetInstance()->Add(id + MAP1, new CSprite(id + MAP1, texture));
	}

	sprite = CSprites::GetInstance()->Get(id + MAP1);
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