#pragma once
#include"GameObject.h"

#define WIDTH_MOVE_HEART 25
#define ITEM_SPEED_FLY 0.05f
#define ITEM_GAME_GRAVITY 0.15f
#define ITEM_LIFE_TIME 5000
#define ITEM_PULL_Y 2

#define HP_INCREASE_CHICKEN	 6
#define AMOUNT_HEART		 1
#define AMOUNT_HEART_BIG	 5

#define HEART_ITEM_SIZE		 D3DXVECTOR2(16,16)
#define HEART_BIG_ITEM_SIZE	 D3DXVECTOR2(24,20)
#define WHIP_ITEM_SIZE		 D3DXVECTOR2(32,32)
#define KNIFE_ITEM_SIZE		 D3DXVECTOR2(30,30)
#define AXE_ITEM_SIZE		 D3DXVECTOR2(30,28)
#define BOOMERANG_ITEM_SIZE	 D3DXVECTOR2(28,28)
#define MONEY_ITEM_SIZE		 D3DXVECTOR2(30,30)
#define GUNPOWDER_ITEM_SIZE	 D3DXVECTOR2(30,30)
#define DOUBLE_WEAPON_SIZE	 D3DXVECTOR2(28,28)
#define CROWM_YELLOW_SIZE	 D3DXVECTOR2(32,32)
#define CHICKEN_SIZE		 D3DXVECTOR2(32,30)
#define END_SIZE			 D3DXVECTOR2(32,32)



class CItem : public CGameObject
{
	float x_backup;
	bool isOnGround;
	ObjectType ItemType;
	DWORD lifetime;
	bool isOverTime;
public:
	CItem(float _x, float _y, ObjectType type);
	~CItem();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	ObjectType GetItemType();
	bool IsOverTime();
	void Death();
};