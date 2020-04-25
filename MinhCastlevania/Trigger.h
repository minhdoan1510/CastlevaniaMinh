#pragma once
#include"GameObject.h"

class CTrigger :public CGameObject
{
	ObjectType triggerType;
	int width, height;
	float x_checkpoint;
	float y_checkpoint;
	int DirectStair;
public:
	CTrigger(int _x, int _y, int _w, int _h, ObjectType _type, int _DirectStair = 0);
	~CTrigger();

	ObjectType GetTriggerType() { return triggerType; }
	float GetX_CheckPoint() { return x_checkpoint; }
	int GetDirectStair() { return DirectStair; }
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

