#pragma once
#include"Effect.h"
#include<vector>
#include<random>

#define SPEED_BRICK_EFFECT 0.01
#define TIME_BRICK_EFFECT 5000

class CBrickEffect:public CEffect
{
	std::vector<pair<float, float>> _direct;
	std::vector<pair<float, float>> _position;
public:
	CBrickEffect(float _x, float _y, ObjectType _type);
	~CBrickEffect();

	void Update(DWORD dt);
};

