#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetIsEndScene()) return;
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetCanDownPipe(true);
		if(!mario->GetIsOnPipe()) mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_UP:
		mario->SetCanUpPipe(true);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TAIL);
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

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{

	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetIsEndScene()) return;

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
	case DIK_A: mario->SetCanHold(false); mario->SetRunLevel(0);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetIsEndScene()) return;

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		if(mario->GetState()!=MARIO_STATE_DOWN_PIPE&& mario->GetState() != MARIO_STATE_UP_PIPE)
			mario->SetState(MARIO_STATE_IDLE);
}