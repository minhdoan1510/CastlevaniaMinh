#include "EnemyDoor.h"
#include "Zombie.h"
#include "BlackKnight.h"
#include "Bat.h"
#include "Bridge.h"
#include "Raven.h"
#include "Ghost.h"
#include "Feaman.h"
#include "WhiteSkeleton.h"
#include "Camera.h"
#include "Simon.h"


CEnemyDoor::CEnemyDoor(float _x, float _y, ObjectType _enemyType, int direct)
{
	x = _x;
	y = _y;
	enemyType = _enemyType;
	objType = ENEMY_DOOR;
	timeSpawn = timeOnceSpawn = 0;
	amountSpawnEnemy = 0;
	enemySpawn = new vector<CGameObject*>();	
	directSpawn = direct;
	timeSpawnDefine = GetRandomInt(ENEMY_DOOR_MIN_TIME_SPAWN, ENEMY_DOOR_MAX_TIME_SPAWN);
}

CEnemyDoor::~CEnemyDoor()
{
}

void CEnemyDoor::Render()
{
	RenderBoundingBox();
}

void CEnemyDoor::Update(DWORD dt, vector<LPGAMEOBJECT>* objects)
{
	RECT rectCam = CCamera::GetInstance()->GetRectCam();
	if (rectCam.right < x && directSpawn == 1) return; // block hưứng đi của quái nếu có
	if (rectCam.left > x && directSpawn == -1) return;
	DWORD now = GetTickCount(); 
	if (amountSpawnEnemy == 0 &&now - timeSpawn >= timeSpawnDefine)
	{
		timeSpawnDefine = GetRandomInt(ENEMY_DOOR_MIN_TIME_SPAWN, ENEMY_DOOR_MAX_TIME_SPAWN);
		timeSpawn = now;
		isSpawn = 1;
		amountSpawnEnemy = GetRandomInt(ENEMY_DOOR_MIN_AMOUNT_SPAWN, ENEMY_DOOR_MAX_AMOUNT_SPAWN);
	}
	if (amountSpawnEnemy > 0)
	{
		if (IsContain(GetBBox(), rectCam))
		{
			amountSpawnEnemy = 0;
			return;
		}
		if (now - timeOnceSpawn >= ENEMY_DOOR_TIME_SPAWN_ONCE)
		{
			amountSpawnEnemy--;
			timeOnceSpawn = now;
			CGameObject* a;
			switch (static_cast<ObjectType>(enemyType))
			{
			case BLACKKNIGHT:
				a = new CBlackKnight(x, y);
				break;
			case BAT:
				a = new CBat(x, y);
				break;
			case GHOST:
				a = new CGhost(x, y);
				break;
			case RAVEN:
				a = new CRaven(x, y);
				break;
			case FEAMAN:
				a = new CFeaman(x, y);
				break;
			case WHITESKELETON:
				a = new CWhiteSkeleton(x, y);
				break;
			case ZOMBIE:
				a = new CZombie(x, y);
				break;
			default:
				a = nullptr;
				break;
			}
			if (a != NULL)
				enemySpawn->push_back(a);
		}
	}
}

vector<CGameObject*>* CEnemyDoor::GetEnemySpawn()
{
	return enemySpawn;
}

void CEnemyDoor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ENEMY_DOOR_BBOX_WIDTH;
	b = y + ENEMY_DOOR_BBOX_HEIGHT;
}
