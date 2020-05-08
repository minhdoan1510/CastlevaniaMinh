#include "Font.h"


CFont::CFont()
{
	sprite = CSprites::GetInstance()->Get(FONT_BOARD);
}

CFont::~CFont()
{
}

void CFont::Draw(string str, int x, int y)
{
	RECT rectTex = sprite->GetRECT();
	RECT rect;
	for (size_t i = 0; i < str.length(); i++)
	{
		rect = rectTex;
		if (str[i] <= 'Z' && str[i] >= 'A')
		{
			rect.left += SIZE_FONT_W * (((int)str[i]) - INT_A_ASCII);
			rect.top += 0;
			rect.right = rect.left + SIZE_FONT_W;
			rect.bottom = rect.top + SIZE_FONT_H;
			sprite->DrawFrame(x + SIZE_FONT_W * i, y, rect);//65-90
		}
		else if (str[i] == '-')
		{
			rect.left += SIZE_FONT_W * INT_EXEPT_ASCII;
			rect.top += 0;
			rect.right = rect.left + SIZE_FONT_W;
			rect.bottom = rect.top + SIZE_FONT_H;
			sprite->DrawFrame(x + SIZE_FONT_W * i, y, rect);
		}
		else if (str[i] >= '0' && str[i] <= '9')
		{
			rect.left += SIZE_FONT_W * (((int)str[i]) - INT_0_ASCII + SIZE_AZ_ASCII);
			rect.top += 0;
			rect.right = rect.left + SIZE_FONT_W;
			rect.bottom = rect.top + SIZE_FONT_H;
			sprite->DrawFrame(x + SIZE_FONT_W * i, y, rect);
		}
		else if (str[i] == ' ')
		{
			rect.left += SIZE_FONT_W * INT_SPACE_ASCII;
			rect.top += 0;
			rect.right = rect.left + SIZE_FONT_W;
			rect.bottom = rect.top + SIZE_FONT_H;
			sprite->DrawFrame(x + SIZE_FONT_W * i, y, rect);
		}
	}
}
