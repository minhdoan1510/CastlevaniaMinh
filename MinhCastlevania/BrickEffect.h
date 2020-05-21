#pragma once
#include"Effect.h"
#include<vector>
#include<random>

#define SPEED_BRICK_EFFECT		0.34
#define GRAVITY_BRICK_EFFECT	0.001f
#define TIME_BRICK_EFFECT		5000
#define AMOUNT_DEBRIS_EFFECT	10
#define SIZE_DEBRIS_EFFECT		10


class CBrickEffect:public CEffect
{
	std::vector<pair<float, float>>		_speed;		//vx,vy
	std::vector<pair<float, float>>		_position;	
public:
	CBrickEffect(float _x, float _y, ObjectType _type, ObjectType _itemHolder);
	~CBrickEffect();

	void Update(DWORD dt);
	void Render();
};

