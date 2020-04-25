#pragma once
#include"Sprites.h"

#define SIZE_FONT_W 15
#define SIZE_FONT_H 14


class CFont
{
private:
	CSprite* sprite;
public:
	CFont();
	~CFont();
	void Draw(string string, int x, int y);
};