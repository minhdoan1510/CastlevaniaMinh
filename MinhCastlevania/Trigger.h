#pragma once
#include"GameObject.h"
#include"Item.h"

#define HEIGHT_DOORPASS		95
#define CHECKPOINT_X	 4

#define CHECKPOINT_X1	 6

class CTrigger :public CGameObject
{
	ObjectType triggerType;
	CItem* itemHolder;
	int width, height;
	float x_checkpoint;
	float y_checkpoint;
	int DirectStair;
	bool isActive;
	LPSPRITE spriteDoor;
public:
	CTrigger(int _x, int _y, int _w, int _h, ObjectType _type, int _DirectStair = 0);
	~CTrigger();
	void SetIsActive(bool _isActive) { isActive = _isActive; }
	ObjectType GetTriggerType() { return triggerType; }
	float GetX_CheckPoint() { return x_checkpoint; }
	int GetDirectStair() { return DirectStair; }
	void SetItemHolder(CItem* _itemHolder) {itemHolder = _itemHolder;}
	CItem* GetItem() { return itemHolder; }
	bool IsActive() { return isActive; }
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

