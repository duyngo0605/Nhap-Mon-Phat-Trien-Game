#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "Arrow.h"
#include "PlayScene.h"
#include "Hud.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{

	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	//pause scene
	CPlayScene* scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	if (scene->GetId() == ID_SCENE_PLAY)
	{
		if (KeyCode == DIK_W)
			scene->Pause();
	}

	if (scene->GetIsPause()) return;

	if (dynamic_cast<CArrow*>(((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer()))
	{
		CArrow* arrow = dynamic_cast<CArrow*>(((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer());
		switch (KeyCode)
		{
		case DIK_UP:
			if (arrow->GetState() == ARROW_STATE_UP)
				arrow->SetState(ARROW_STATE_DOWN);
			else
				arrow->SetState(ARROW_STATE_UP);
			break;
		case DIK_DOWN:
			if (arrow->GetState() == ARROW_STATE_UP)
				arrow->SetState(ARROW_STATE_DOWN);
			else
				arrow->SetState(ARROW_STATE_UP);
			break;
		case DIK_S:
			arrow->SelectOpion();
			break;
		}
		return;
	}
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (CHud::GetInstance()->isRenderStartDialog && mario->GetIsInWorldMap()) return;
	if (mario->GetIsEndScene()) return;
	switch (KeyCode)
	{
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	}
	if (mario->GetIsInWorldMap()&&!mario->IsMoving())
	{
		switch (KeyCode)
		{
		case DIK_RIGHT:
			if(mario->canMoveRight)
				mario->SetState(MARIO_STATE_WORLDMAP_WALK_RIGHT);
			break;
		case DIK_LEFT:
			if (mario->canMoveLeft)
				mario->SetState(MARIO_STATE_WORLDMAP_WALK_LEFT);
			break;
		case DIK_DOWN:
			if(mario->canMoveDown)
				mario->SetState(MARIO_STATE_WORLDMAP_WALK_DOWN);
			break;
		case DIK_UP:
			if (mario->canMoveUp)
				mario->SetState(MARIO_STATE_WORLDMAP_WALK_UP);
			break;
		case DIK_S:
			mario->EnterNode();
			break;
		}
	}
	else
	{
		switch (KeyCode)
		{
		case DIK_DOWN:
			mario->SetCanDownPipe(true);
			if (!mario->GetIsOnPipe())
				mario->SetState(MARIO_STATE_SIT);
			break;
		case DIK_UP:
			mario->SetCanUpPipe(true);
			break;
		case DIK_S:
			mario->SetState(MARIO_STATE_JUMP);
			break;
		case DIK_4:
			mario->SetPosition(2600.0f, 400.0f);
			break;
		case DIK_0:
			mario->SetState(MARIO_STATE_DIE);
			break;
		case DIK_R: // reset
			//Reload();
			break;
		case DIK_A:
			if (mario->GetRunLevel() < 2)
			{
				mario->SetRunLevel(1);
				mario->SetCanHold(true);
			}
			if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			{
				mario->TailAttack();
			}
			break;
		
		}
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{

	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	if (dynamic_cast<CArrow*>(((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer()))
		return;
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetIsEndScene()) return;
	if (mario->GetIsInWorldMap()) return;
	switch (KeyCode)
	{
	case DIK_S:
		if (!mario->GetIsUsingPipe()) mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		if(!mario->GetIsUsingPipe())	mario->SetState(MARIO_STATE_SIT_RELEASE);
		mario->SetCanDownPipe(false);
		break;
	case DIK_UP:
		mario->SetCanUpPipe(false);
		break;
	
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();

	if (dynamic_cast<CArrow*>(((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer()))
		return;

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetIsEndScene()) return;
	if (mario->GetIsInWorldMap()) return;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetCanHold(true);
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else
		{
			mario->SetCanHold(false);
			mario->SetRunLevel(0);
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetCanHold(true);
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		}
		else
		{
			mario->SetCanHold(false);
			mario->SetRunLevel(0);
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
	}
	else
		if(mario->GetState()!=MARIO_STATE_DOWN_PIPE&& mario->GetState() != MARIO_STATE_UP_PIPE)
			mario->SetState(MARIO_STATE_IDLE);
}