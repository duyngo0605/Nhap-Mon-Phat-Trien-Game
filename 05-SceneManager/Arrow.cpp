#include "Arrow.h"
#include "PlayScene.h"
#include "Game.h"
#include "Data.h"

void CArrow::Render()
{
	CAnimations* animation = CAnimations::GetInstance();
	animation->Get(ID_ANI_ARROW)->Render(x, y);
}

void CArrow::SetState(int state)
{
	switch (state)
	{
	case ARROW_STATE_UP:
		SetPosition(x, y - distance);
		break;
	case ARROW_STATE_DOWN:
		SetPosition(x, y + distance);
		break;
	}
	CGameObject::SetState(state);
}

void CArrow::SelectOpion()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (scene->GetId() == ID_SCENE_WORLDMAP)
	{
		if (state == ARROW_STATE_DOWN)
		{
			CGame::GetInstance()->InitiateSwitchScene(ID_SCENE_INTRO);
			CData::GetInstance()->ResetData();
		}
		else
		{
			CData::GetInstance()->ResetData();
		}

	}
	else
		CGame::GetInstance()->InitiateSwitchScene(ID_SCENE_WORLDMAP);
}
