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
		if (scene->GetId()==ID_SCENE_PLAY)
		{
			x = scene->GetCamX() + HUD_WIDTH / 2;
			y = scene->GetCamY() - HUD_WIDTH / 2-SCREEN_HEIGHT;
			
		}
		else
		{
			x = scene->GetCamX() + HUD_WIDTH / 2;
			y = SCREEN_HEIGHT - HUD_HEIGHT*2;
			
		}
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
}

void CHud::RenderCoin()
{
	CSprites* sprites = CSprites::GetInstance();
	if (CData::GetInstance()->coin < 10)
		sprites->Get(CData::GetInstance()->coin)->Draw(x + 25.0f, y - 4.0f);
	else
	{
		int	unitDigit = CData::GetInstance()->coin % 10;
		int tensDigit = CData::GetInstance()->coin / 10;	
		sprites->Get(unitDigit)->Draw(x + 25.0f, y - 4.0f);
		sprites->Get(tensDigit)->Draw(x + 17.0f, y - 4.0f);
	}
}
