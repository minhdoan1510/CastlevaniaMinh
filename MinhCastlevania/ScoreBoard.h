#pragma once
#include"Animations.h"
#include "Font.h"
#include "Heal.h"
#include <string>

#define BOARD_X_POSITION_DEFAULT 0
#define BOARD_Y_POSITION_DEFAULT -80
#define LENGTH_SCORE 6
#define SCORE_POSITION D3DXVECTOR2(13,15)
#define PLAYER_POSITION D3DXVECTOR2(13,34)
#define ENEMY_POSITION D3DXVECTOR2(11,52)
#define TIME_POSITION D3DXVECTOR2(228,15)
#define STAGE_POSITION D3DXVECTOR2(382,15)
#define	HEART_POSITION D3DXVECTOR2(379,33)
#define	LIFE_POSITION D3DXVECTOR2(364,51)
#define FRAME_ITEM_POSITION D3DXVECTOR2(301,32)
#define HEART_ITEM_POSITION D3DXVECTOR2(362,34)
#define HEAL_SIMON_POSITION D3DXVECTOR2(120,36)
#define HEAL_ENEMY_POSITION D3DXVECTOR2(120,54)
#define WEAPON_POSITION D3DXVECTOR2(308,37)
#define ITEM_AMOUNT_WEAPON_POSITION D3DXVECTOR2(440,30)


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
	int amount2ndWeapon;

public:
	CScoreBoard();
	~CScoreBoard();

	void Render();
	void Update(int _time, int _score, int _heart, int _lifeSimon, int _hpSimon, int _hpEnemy, int _stage, ObjectType _secondweaponCurrent,int _amount2ndWeapon);
};