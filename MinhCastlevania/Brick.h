#pragma once
#include "GameObject.h" 
#include "TextureManager.h"
#include "Camera.h"


class CBrick : public CGameObject
{
private:
	int width;
	int height;
	ObjectType typeBrick;
	ObjectType itemHolder;
public:
	CBrick(float X, float Y, int W, int H, int Model, ObjectType _item = Null);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetTypeBrick();
	void Death();
	ObjectType GetItemHolder();
};
