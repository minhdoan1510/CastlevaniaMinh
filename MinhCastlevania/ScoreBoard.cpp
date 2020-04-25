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
	D3DXVECTOR2 pos = CCamera::GetInstance()->Transform(x, y);
	pos.x = CCamera::GetInstance()->GetXCam();
	pos.y = CCamera::GetInstance()->GetYCam()+ BOARD_Y_POSITION_DEFAULT;
	string temp;
	for (int i = 0; i < 6 - std::to_string(score).length(); i++)
		temp += '0';
	font->Draw("SCORE-"+ temp + to_string(score), 13 + pos.x, 15 + pos.y);
	font->Draw("PLAYER", 13 + pos.x, 34 + pos.y);
	font->Draw("ENEMY", 11 + pos.x, 52 + pos.y);
	font->Draw("TIME", 228 + pos.x, 15 + pos.y);
	font->Draw("STAGE", 382 + pos.x, 15 + pos.y);
	font->Draw("-" + std::to_string(heart), 379 + pos.x, 33 + pos.y);
	font->Draw("P-" + std::to_string(lifeSimon), 364 + pos.x, 51 + pos.y);
	CSprites::GetInstance()->Get(FRAME_ITEM_BOARD)->Draw(pos.x + 301, pos.y + 32, 255);
	CSprites::GetInstance()->Get(HEART_ITEM_BOARD)->Draw(pos.x + 362, pos.y + 34);

	font->Draw(std::to_string(time), 300 + pos.x, 15 + pos.y);
	

	font->Draw('0' + std::to_string(stage), 471 + pos.x, 15 + pos.y);

	heal->Draw(120 + pos.x, 36 + pos.y, hpSimon, 1);
	heal->Draw(120 + pos.x, 54 + pos.y, hpSimon, 0);

	switch (secondweaponCurrent)
	{
	case KNIFE:
		CSprites::GetInstance()->Get(KNIFE_ITEM)->Draw(308 + pos.x, 37 + pos.y);
		break;
	case AXE:
		CSprites::GetInstance()->Get(AXE_ITEM)->Draw(308 + pos.x, 37 + pos.y);
		break;
	case BOOMERANG:
		CSprites::GetInstance()->Get(BOOMERANG_ITEM)->Draw(308 + pos.x, 37 + pos.y);
		break; 
	case GUNPOWDER:
		CSprites::GetInstance()->Get(GUNPOWDER_ITEM)->Draw(308 + pos.x, 37 + pos.y);
		break;
	default:
		break;
	}

}

void CScoreBoard::Update(int _time, int _score, int _heart, int _lifeSimon, int _hpSimon, int _hpEnemy, int _stage, ObjectType _secondweaponCurrent)
{
	secondweaponCurrent = _secondweaponCurrent;
	time =_time;
	score =_score;
	heart = _heart;
	lifeSimon =_lifeSimon;
	hpSimon = _hpSimon;
	hpEnemy = _hpEnemy;
	stage = _stage;
}