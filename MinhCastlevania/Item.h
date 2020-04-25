#pragma once
#include"GameObject.h"

#define WIDTH_MOVE_HEART 25
#define ITEM_SPEED_FLY 0.05f
#define ITEM_GAME_GRAVITY 0.15f
#define ITEM_LIFE_TIME 5000

class CItem : public CGameObject
{
	float x_backup;
	bool isOnGround;
	ObjectType ItemType;
	DWORD lifetime;
public:
	CItem(float _x, float _y, ObjectType type);
	~CItem();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	ObjectType GetItemType();
	void Death();
};