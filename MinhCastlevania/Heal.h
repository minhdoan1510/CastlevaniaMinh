#pragma once
#include"Sprites.h"

#define HEAL_WIDTH_FRAME 8
#define HEAL_HEIGHT_FRAME 13


class CHeal
{
	CSprite* sprite;
public:
	CHeal();
	~CHeal();
	void Draw(int x, int y, int heal, bool isPlayer);
};

