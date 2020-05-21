#pragma once
#include"GameObject.h"

#define CELL_SIZE D3DXVECTOR2(256,192)

class CGrid
{
	//vector<CGameObject*>** cells;
	vector<CGameObject*> cells[20][20];
	int columnGrid, rowGrid;
	float cellW, cellH;
	vector<CGameObject*> FilterObjectDuplicate(vector<CGameObject*> obj);
	string fileobj;
	D3DXVECTOR2 posSimonDefault;
public:
	CGrid(int _mapW, int _mapH, string fileobj);
	void LoadGrid();
	~CGrid();
	void UpdateGrid(vector<CGameObject*> objects);
	void RemoveObj(CGameObject* obj, bool isDeletePointer = 1);
	void UnLoadGrid();
	void InsertGrid(CGameObject* obj);

	D3DXVECTOR2 GetPosSimonDefault();
	vector<CGameObject*> GetListObj();
};

