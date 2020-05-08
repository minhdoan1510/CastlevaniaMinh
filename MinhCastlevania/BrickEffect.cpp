#include "BrickEffect.h"

CBrickEffect::CBrickEffect(float _x, float _y, ObjectType _type):CEffect::CEffect(_x, _y, _type)
{
	random_device rd;
	mt19937 rng(rd());	
	uniform_real_distribution<float> minh(-1, 1);

	//sprite = CSprites::GetInstance()->Get(DEBRIS);
	for (int i = 0; i < 10; i++)
	{
		_direct.push_back(make_pair<float, float>(minh(rng), minh(rng)));
	}
}

CBrickEffect::~CBrickEffect()
{

}

void CBrickEffect::Update(DWORD dt)
{
	if (GetTickCount() - lifeTime > TIME_BRICK_EFFECT)
	{
		//IsDead = true;
		return;
	}
	for (int i = 0; i < _direct.size(); i++)
	{
		if (_direct.at(i).first > 0)
		{

		}
	}
}
