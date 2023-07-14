#pragma once
#include "Node.h"
#include "Card.h"
#include "PlayScene.h"

#define LIMIT_TIME 300

#define MARIO_WOLRDMAP_START_X	24.0f
#define MARIO_WOLRDMAP_START_Y	40.0f

///store the data to link 1-1 and worldmap HUD
class CData
{
	static CData* __dataInstance;
public:

	static int coin;
	static int HP;
	static int score;
	static CCard cardGot[3];
	static int marioLevel;
	static int timer;
	static float xMarioWorld;
	static float yMarioWorld;
	static int marioRunLevel;
	CData(){}
	~CData(){}
	static CData* GetInstance()
	{
		if (!__dataInstance)
			__dataInstance = new CData();
		return __dataInstance;
	}
	void SetCard(int type);
	void ResetData()
	{
		coin = score = 0;
		HP = 4;
		for (int i = 0; i < 3; i++)
			cardGot[i] = CCard();
		timer = LIMIT_TIME;
		xMarioWorld = MARIO_WOLRDMAP_START_X;
		yMarioWorld = MARIO_WOLRDMAP_START_Y;
		marioRunLevel = 0;
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		if (scene->GetId() == ID_SCENE_WORLDMAP)
		{
			scene->GetPlayer()->SetPosition(-100.0f, -100.0f);
			CMario* mario = new CMario(MARIO_WOLRDMAP_START_X, MARIO_WOLRDMAP_START_Y);
			scene->SetPlayer(mario);
			scene->AddObject(mario);
		}
	}
};

