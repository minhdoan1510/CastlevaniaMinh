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
	CMap(int _id, string fileMap);
	~CMap();
	void ReadFileMap(string filePath);
	void DrawMap();
	void DrawMapTransform(float xTrans, float yTrans, int directTrans);
	void DrawIntroScene();
	void LoadMap(string fileTileMap);
	int GetMapWidth();
	int GetMapHeight();
	int GetFloorMap();
	void SetSpriteTex(CSprite* s, LPDIRECT3DTEXTURE9 texture);
	void SetBoundaryLeftRight(int floor);
};

