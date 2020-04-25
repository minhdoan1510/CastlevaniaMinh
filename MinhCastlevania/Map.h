#pragma once
#include "TextureManager.h"
#include "Sprites.h"
#include "GameDefine.h"
#include "Camera.h"

class CMap
{
private:
	int id;
	LPDIRECT3DTEXTURE9 texture;
	CSprite* sprite;
	int columnMap, rowMap, floor;
	int _rowTile, _columnTile;
	int _idTileTransparent;
	int sizeFrame;
	int tileMap[200][200];
public:
	CMap(int _id, LPCWSTR fileMap);
	~CMap();
	void ReadFileMap(LPCWSTR filePath);
	void DrawMap();
	void LoadMap();	
	int GetMapWidth();
	int GetMapHeight();
	int GetFloorMap();
	void SetBoundaryLeftRight(int floor);
};

