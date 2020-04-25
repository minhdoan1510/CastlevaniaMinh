#pragma once
#include"Animations.h"
#include "Font.h"
#include "Heal.h"
#include <string>

#define BOARD_X_POSITION_DEFAULT 4
#define BOARD_Y_POSITION_DEFAULT -80

class CScoreBoard
{
	float x, y;
	CFont* font;
	CHeal* heal;
	ObjectType secondweaponCurrent;
	int time;
	int score;
	int heart;
	int lifeSimon;
	int hpSimon;
	int hpEnemy;
	int stage;

public:
	CScoreBoard();
	~CScoreBoard();

	void Render();
	void Update(int _time, int _score, int _heart, int _lifeSimon, int _hpSimon, int _hpEnemy, int _stage, ObjectType _secondweaponCurrent);
};