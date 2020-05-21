#include "BrickEffect.h"
#include "Camera.h"

CBrickEffect::CBrickEffect(float _x, float _y, ObjectType _type, ObjectType _itemHolder):CEffect::CEffect(_x, _y, BRICK_EFFECT, TIME_BRICK_EFFECT,_itemHolder)
{
	random_device rd;
	mt19937 rng(rd());	
	uniform_real_distribution<float> minh(-1, 1);

	sprite = CSprites::GetInstance()->Get(DEBRIS);
	for (int i = 0; i < AMOUNT_DEBRIS_EFFECT; i++)
	{
		_speed.push_back(make_pair((float)minh(rng) * SPEED_BRICK_EFFECT, (float)minh(rng) * SPEED_BRICK_EFFECT));
		_position.push_back(make_pair(_x, _y));
	}
}

CBrickEffect::~CBrickEffect()
{

}

void CBrickEffect::Update(DWORD dt)
{
	float dx, dy;
	if (_speed.size() ==0)
	{
		isFinish = true;
	}

	RECT rectCam = CCamera::GetInstance()->GetRectCam(); 
	int k = 0;
	for (int i = 0; i < _speed.size()-k; i++)
	{
		if (i >= _speed.size())
			break;
		dx = _speed.at(i).first * dt;
		dy = _speed.at(i).second * dt;
		_speed.at(i).second += GRAVITY_BRICK_EFFECT * dt;
		_position.at(i).first += dx;
		_position.at(i).second += dy;
		RECT rect;
		rect.left = _position.at(i).first;
		rect.top = _position.at(i).second;
		rect.right = rect.left + SIZE_DEBRIS_EFFECT;
		rect.bottom = rect.top + SIZE_DEBRIS_EFFECT;
		if (!CCamera::GetInstance()->IsContainCam(rect))
		{
			_speed.erase(_speed.begin() + i);
			_position.erase(_position.begin() + i);
			k++;
		}
	}
}

void CBrickEffect::Render()
{
	for (int i = 0; i <_position.size(); i++)
	{
		sprite->Draw(_position.at(i).first, _position.at(i).second);
	}
}
