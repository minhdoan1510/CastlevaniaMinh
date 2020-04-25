#include "MapManager.h"
#include "GameDefine.h"

CMapManager* CMapManager::intance = NULL;

CMapManager::CMapManager()
{
}

CMapManager::~CMapManager()
{
}

CMapManager* CMapManager::GetIntance()
{
	if (intance == NULL)
		intance = new CMapManager();
	return intance;
}

CMap* CMapManager::Get(int id)
{
	CMap* m = maps[id];
	return m;
}

void CMapManager::Add(int id,CMap* map)
{
	maps[id] = map;
	DebugOut(L"[MapManager] Added content map id: %d \n", id);
}
