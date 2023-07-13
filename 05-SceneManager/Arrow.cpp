#include "Arrow.h"
#include "PlayScene.h"
#include "Game.h"

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
		SetPosition(ARROW_X,ARROW_UP_Y);
		break;
	case ARROW_STATE_DOWN:
		SetPosition(ARROW_X, ARROW_DOWN_Y);
		break;
	}
	CGameObject::SetState(state);
}

void CArrow::EnterWorldMap()
{
	CGame::GetInstance()->InitiateSwitchScene(ID_SCENE_WORLDMAP);
}
