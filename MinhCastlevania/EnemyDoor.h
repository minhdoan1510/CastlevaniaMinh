#pragma once
#include "GameObject.h"
#include "Zombie.h"

#define ENEMY_DOOR_BBOX_WIDTH		 4
#define ENEMY_DOOR_BBOX_HEIGHT		 64
#define ENEMY_DOOR_TIME_SPAWN_ONCE	 600
#define ENEMY_DOOR_MIN_TIME_SPAWN	 4500
#define ENEMY_DOOR_MAX_TIME_SPAWN	 6500
#define ENEMY_DOOR_MIN_AMOUNT_SPAWN	 1
#define ENEMY_DOOR_MAX_AMOUNT_SPAWN	 3


class CEnemyDoor:public CGameObject
{
	DWORD timeSpawn;
	DWORD timeOnceSpawn;
	DWORD timeSpawnDefine;
	ObjectType enemyType;
	vector<CGameObject*>* enemySpawn;
	bool isSpawn;
	int amountSpawnEnemy;
	int directSpawn;
public:
	CEnemyDoor(float _x, float _y,ObjectType _enemyType, int direct);
	~CEnemyDoor();
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects = NULL);
	vector<CGameObject*>* GetEnemySpawn();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

