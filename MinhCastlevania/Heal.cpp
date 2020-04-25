#include "Heal.h"

CHeal::CHeal()
{
	sprite = CSprites::GetInstance()->Get(HEAL_ITEM_BOARD);
}

CHeal::~CHeal()
{
}

void CHeal::Draw(int x, int y, int heal, bool isPlayer)
{
	RECT rectTex = sprite->GetRECT();
	RECT rect;
	for (int i = 0; i < HEAL_DEFAULT; i++)
	{
		rect = rectTex;
		if (i < heal)
		{
			if (isPlayer)
			{
				rect.left += HEAL_WIDTH_FRAME * 0;
				rect.top += 0;
				rect.right = rect.left + HEAL_WIDTH_FRAME;
				rect.bottom = rect.top + HEAL_HEIGHT_FRAME;
				sprite->DrawFrame(x + i * HEAL_WIDTH_FRAME, y, rect);
			}
			else
			{
				rect.left += HEAL_WIDTH_FRAME * 2;
				rect.top += 0;
				rect.right = rect.left + HEAL_WIDTH_FRAME;
				rect.bottom = rect.top + HEAL_HEIGHT_FRAME;
				sprite->DrawFrame(x + i * HEAL_WIDTH_FRAME, y, rect);
			}
		}
		else
		{
			rect.left += HEAL_WIDTH_FRAME * 1;
			rect.top += 0;
			rect.right = rect.left + HEAL_WIDTH_FRAME;
			rect.bottom = rect.top + HEAL_HEIGHT_FRAME;
			sprite->DrawFrame(x + i * HEAL_WIDTH_FRAME, y, rect);
		}
	}
}
