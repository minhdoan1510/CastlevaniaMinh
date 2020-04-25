#pragma once
#include"Map.h"

class CMapManager
{
	unordered_map<int, CMap*> maps;
	static CMapManager* intance;
	CMapManager();
public:
	~CMapManager();
	static CMapManager* GetIntance();
	CMap* Get(int id);
	void Add(int id, CMap* map);
};

