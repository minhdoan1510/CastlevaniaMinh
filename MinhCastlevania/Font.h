#pragma once
#include"Sprites.h"

#define SIZE_FONT_W 15
#define SIZE_FONT_H 14
#define INT_A_ASCII 65
#define INT_EXEPT_ASCII 36
#define INT_0_ASCII 48
#define SIZE_AZ_ASCII 26
#define INT_SPACE_ASCII 37

class CFont
{
private:
	CSprite* sprite;
public:
	CFont();
	~CFont();
	void Draw(string string, int x, int y);
};