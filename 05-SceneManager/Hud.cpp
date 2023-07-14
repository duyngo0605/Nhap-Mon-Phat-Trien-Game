#include "Hud.h"

#include "Game.h"
#include "PlayScene.h"
#include "Data.h"

CHud* CHud::__hudInstance = nullptr;

CHud* CHud::GetInstance()
{
	if (!__hudInstance)
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		float x, y;
		x = scene->GetCamX() + HUD_WIDTH / 2;
		y = scene->GetCamY() - HUD_WIDTH / 2 - SCREEN_HEIGHT;
		__hudInstance = new CHud(x, y);
	}
	return __hudInstance;
}

void CHud::Update()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	x = scene->GetCamX() + CGame::GetInstance()->GetBackBufferWidth() / 2;
	y = scene->GetCamY() + CGame::GetInstance()->GetBackBufferHeight() - HUD_HEIGHT/2;
}

void CHud::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(ID_HUD)->Draw(x, y);
	sprites->Get(ID_NUMBER_1)->Draw(x - 77.0f, y - 4.0f);
	RenderCoin();
	RenderHP();
	RenderScore();
	RenderTimer();
	RenderSpeedBar();
	RenderCard();
	RenderGameOverHud();
	if (GetTickCount64() - spawn_start > DIALOG_TIMEOUT)
	{	
		isRenderStartDialog = false;
		return;
	}
	RenderStartHud();


}

void CHud::RenderCoin()
{
	CSprites* sprites = CSprites::GetInstance();
	if (CData::GetInstance()->coin < 10)
		sprites->Get(CData::GetInstance()->coin)->Draw(x + 25.0f, y - 4.0f);
	else
	{
		int	unitsDigit = CData::GetInstance()->coin % 10;
		int tensDigit = CData::GetInstance()->coin / 10;	
		sprites->Get(unitsDigit)->Draw(x + 25.0f, y - 4.0f);
		sprites->Get(tensDigit)->Draw(x + 17.0f, y - 4.0f);
	}
}

void CHud::RenderHP()
{
	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(CData::GetInstance()->HP)->Draw(x - 84.0f, y+5.0f);
}

void CHud::RenderScore()
{
	CSprites* sprites = CSprites::GetInstance();
	int digits[7];
	int Score= CData::GetInstance()->score;
	for (int i = 0; i < 7; i++)
	{
		digits[i] = Score % 10;
		Score /= 10;
	}
	for (int i = 0; i < 7; i++)
	{
		sprites->Get(digits[i])->Draw(x - 8.0f - 9.0f * i, y + 5.0f);
	}
}

void CHud::RenderTimer()
{
	CSprites* sprites = CSprites::GetInstance();
	int digits[3];
	int Timer=CData::GetInstance()->timer;
	for (int i = 0; i < 3; i++)
	{
		digits[i] = Timer % 10;
		Timer /= 10;
	}
	for (int i = 0; i < 3; i++)
	{
		sprites->Get(digits[i])->Draw(x +26.0f - 8.0f * i, y + 4.0f);
	}
}

void CHud::RenderSpeedBar()
{
	CSprites* sprites = CSprites::GetInstance();
	if (CData::GetInstance()->marioRunLevel == 1)
	{
		sprites->Get(ID_RUNLEVEL_1)->Draw(x - 34.0f, y - 3.0f);
	}
	else if (CData::GetInstance()->marioRunLevel == 2)
	{
		sprites->Get(ID_RUNLEVEL_2)->Draw(x - 34.0f, y - 3.0f);
	}
}

void CHud::RenderCard()
{
	for (int i = 0; i < 3; i++)
	{
		CAnimations* animations = CAnimations::GetInstance();
		switch (CData::GetInstance()->cardGot[i].GetType())
		{
		case CARD_TYPE_MUSHROOM:
			animations->Get(ID_ANI_CARD_MUSHROOM + 200)->Render(x + 54.0f + i * 24.0f, y);
			break;
		case CARD_TYPE_STAR:
			animations->Get(ID_ANI_CARD_STAR + 200)->Render(x + 54.0f + i * 24.0f, y);
			break;
		case CARD_TYPE_FLOWER:
			animations->Get(ID_ANI_CARD_FLOWER + 200)->Render(x + 54.0f + i * 24.0f, y);
			break;
		}
	}
}

void CHud::RenderStartHud()
{
	
	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(ID_DIALOG_START)->Draw(DIALOG_X, DIALOG_Y);
	sprites->Get(CData::GetInstance()->HP)->Draw(DIALOG_X + 39.0f, DIALOG_Y + 10.0f);
}

void CHud::RenderGameOverHud()
{
	if (CData::GetInstance()->HP == 0)
	{
		CSprites* sprites = CSprites::GetInstance();
		sprites->Get(ID_DIALOG_END)->Draw(DIALOG_X, DIALOG_Y);
	}
}
