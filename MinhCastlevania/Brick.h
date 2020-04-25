#pragma once
#include "GameObject.h" 
#include "TextureManager.h"
#include "Camera.h"

#define BRICK_MODEL_TRANSPARENT_1 4
#define BRICK_MODEL_TRANSPARENT_2 5
#define TEX_BRICK_MAP_2 6
#define TEX_BRICK_MAP_3 7

class CBrick : public CGameObject
{
private:
	int width;
	int height;
	ObjectType typeBrick;

public:
	CBrick(float X, float Y, int W, int H, int Model);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetTypeBrick();
};
