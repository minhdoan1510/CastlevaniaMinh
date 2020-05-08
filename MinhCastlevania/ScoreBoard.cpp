#include "ScoreBoard.h"
#include "Camera.h"

CScoreBoard::CScoreBoard()
{
	x = BOARD_X_POSITION_DEFAULT;	
	y = BOARD_Y_POSITION_DEFAULT;
	font = new CFont();
	heal = new CHeal();
}

CScoreBoard::~CScoreBoard()
{
}

void CScoreBoard::Render()
{
	D3DXVECTOR2 pos;// = CCamera::GetInstance()->Transform(x, y);
	pos.x = CCamera::GetInstance()->GetXCam()+ BOARD_X_POSITION_DEFAULT;
	pos.y = CCamera::GetInstance()->GetYCam()+ BOARD_Y_POSITION_DEFAULT;
	string temp;
	for (int i = 0; i < LENGTH_SCORE - std::to_string(score).length(); i++)
		temp += '0';

	font->Draw("SCORE-"+ temp + to_string(score), SCORE_POSITION.x + pos.x, SCORE_POSITION.y + pos.y);
	font->Draw("PLAYER", PLAYER_POSITION.x + pos.x, PLAYER_POSITION.y + pos.y);
	font->Draw("ENEMY", ENEMY_POSITION.x + pos.x, ENEMY_POSITION.y + pos.y);
	font->Draw("TIME " + std::to_string(time), TIME_POSITION.x + pos.x, TIME_POSITION.y + pos.y);
	font->Draw("STAGE 0"+ std::to_string(stage), STAGE_POSITION.x + pos.x, STAGE_POSITION.y + pos.y);
	font->Draw("-" + std::to_string(heart), HEART_POSITION.x + pos.x, HEART_POSITION.y + pos.y);
	font->Draw("P-" + std::to_string(lifeSimon), LIFE_POSITION.x + pos.x, LIFE_POSITION.y + pos.y);
	CSprites::GetInstance()->Get(FRAME_ITEM_BOARD)->Draw(pos.x + FRAME_ITEM_POSITION.x, pos.y + FRAME_ITEM_POSITION.y, 255);
	CSprites::GetInstance()->Get(HEART_ITEM_BOARD)->Draw(pos.x + HEART_ITEM_POSITION.x, pos.y + HEART_ITEM_POSITION.y);


	heal->Draw(HEAL_SIMON_POSITION.x + pos.x, HEAL_SIMON_POSITION.y + pos.y, hpSimon, 1);
	heal->Draw(HEAL_ENEMY_POSITION.x + pos.x, HEAL_ENEMY_POSITION.y + pos.y, hpEnemy, 0);

	switch (amount2ndWeapon)
	{
	case 2:
		CSprites::GetInstance()->Get(DOUBLE_WEAPON)->Draw(ITEM_AMOUNT_WEAPON_POSITION.x + pos.x, ITEM_AMOUNT_WEAPON_POSITION.y + pos.y);
		break;
	case 3:	
		CSprites::GetInstance()->Get(TRIPLE_WEAPON)->Draw(ITEM_AMOUNT_WEAPON_POSITION.x + pos.x, ITEM_AMOUNT_WEAPON_POSITION.y + pos.y);
		break;
	}

	pos += WEAPON_POSITION;
	switch (secondweaponCurrent)
	{
	case KNIFE:
		CSprites::GetInstance()->Get(KNIFE_ITEM)->Draw(pos.x, pos.y);
		break;
	case AXE:
		CSprites::GetInstance()->Get(AXE_ITEM)->Draw(pos.x,pos.y);
		break;
	case BOOMERANG:
		CSprites::GetInstance()->Get(BOOMERANG_ITEM)->Draw(pos.x,pos.y);
		break; 
	case GUNPOWDER:
		CSprites::GetInstance()->Get(GUNPOWDER_ITEM)->Draw(pos.x,pos.y);
		break;
	default:
		break;
	}

}

void CScoreBoard::Update(int _time, int _score, int _heart, int _lifeSimon, int _hpSimon, int _hpEnemy, int _stage, ObjectType _secondweaponCurrent, int _amount2ndWeapon)
{
	secondweaponCurrent = _secondweaponCurrent;
	time =_time;
	score =_score;
	heart = _heart;
	lifeSimon =_lifeSimon;
	hpSimon = _hpSimon;
	hpEnemy = _hpEnemy;
	stage = _stage;
	amount2ndWeapon = _amount2ndWeapon;
}