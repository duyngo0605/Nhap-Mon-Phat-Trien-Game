#include "Data.h"
#include "Mario.h"

CData* CData::__dataInstance = nullptr;
int CData::coin = 0;
int CData::HP = 4;
int CData::score = 0;
CCard CData::cardGot[3];
int CData::marioLevel = MARIO_LEVEL_SMALL;
int CData::timer = LIMIT_TIME;
int CData::marioRunLevel = 0;
float CData::xMarioWorld = MARIO_WOLRDMAP_START_X;
float CData::yMarioWorld = MARIO_WOLRDMAP_START_Y;

void CData::SetCard(int type)
{
	for (int i = 0; i < 3; i++)
	{
		if (cardGot[i].GetType() == -1)
		{
			cardGot[i] = CCard(-100.0f, -100.0f, type, CARD_STATE_HUD);
			break;
		}
	}
}