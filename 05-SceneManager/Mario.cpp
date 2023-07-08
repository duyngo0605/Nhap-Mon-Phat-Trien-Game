#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#include "Goomba.h"
#include "Coin.h"
#include "QuestionBrick.h"
#include "Mushroom.h"
#include "Portal.h"
#include "SpecialPlatform.h"
#include "Leaf.h"
#include "FireVenusTrap.h"
#include "FireBall.h"
#include "Koopa.h"
#include"SpecialBrick.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	if (GetTickCount64() - untouchable_start >= MARIO_UNTOUCHABLE_TIME/2)
	{
		if (GetTickCount64() - untouchable_start >= MARIO_UNTOUCHABLE_TIME)
		{

			untouchable_start = 0;
			untouchable = 0;
		}
		isTransforming = false;

	}

	if (GetTickCount64() - kick_start >= MARIO_KICKING_TIME)
	{
		isKicking = false;
	}
	if (GetTickCount64() - tailAttack_start >= MARIO_TAIL_ATTACK_TIME)
	{
		isAttacking = false;
	}
	if (GetTickCount64() - flyJump_start >= MARIO_FLY_JUMP_TIME)
	{
		flyJump = false;
		ay = MARIO_GRAVITY;
	}
	if (isOnPlatform) { isFlying = false; ay = 0.002f; }
	else
		ay = MARIO_GRAVITY;
	if (isTransforming) vx = vy = 0;
	else
	{
		vy += ay * dt;
		vx += ax * dt;
	}

	if (abs(vx) >= abs(maxVx)) { vx = maxVx; SetRunLevel(2); }
	else
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X) SetRunLevel(1);
		else
			SetRunLevel(0);
	}
	if (x <= MARIO_SMALL_BBOX_WIDTH/2)x = MARIO_SMALL_BBOX_WIDTH / 2;
	if (x >= MAP_WIDTH - MARIO_SMALL_BBOX_WIDTH / 2)x = MAP_WIDTH - MARIO_SMALL_BBOX_WIDTH / 2;
	if (y >= HEIGHT_DEATH) SetState(MARIO_STATE_DIE);
	else if (y <= 0) {
		y = MARIO_BIG_BBOX_HEIGHT/2;
		vy = 0.0001f;
		if (nx > 0) SetState(MARIO_STATE_WALKING_RIGHT);
		else
			SetState(MARIO_STATE_WALKING_LEFT);
	}
	// reset untouchable timer if untouchable time has passed
	
	
	isOnPlatform = false;

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0)
		{
			isOnPlatform = true;
		}
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}
	if (dynamic_cast<CSpecialPlatform*>(e->obj))
		OnCollisionWithSpecialPlatform(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CSpecialBrick*>(e->obj))
		OnCollisionWithSpecialBrick(e);
	else if (dynamic_cast<CQuestionBrick*>(e->obj))
		OnCollisionWithQuestionBrick(e);
	else if (dynamic_cast<CMushRoom*>(e->obj))
		OnCollisionWithMushRoom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CFireVenusTrap*>(e->obj))
		OnCollisionWithFireVenusTrap(e);
	else if (dynamic_cast<CFireBall*>(e->obj))
		OnCollisionWithFireBall(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->MinusLevel();
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (isAttacking)
		{
			goomba->SetState(GOOMBA_STATE_TAIL_ATTACKED);
			goomba->SetSpeed(nx*MARIO_TAIL_ATTACK_SPEED_X, MARIO_TAIL_ATTACK_SPEED_Y);
		}
		else
		{
			if (untouchable == 0)
			{
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						SetLevel(level - 1);
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(MARIO_STATE_DIE);
					}
				}
			}
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	isHolding = false;
	// jump on top >> Koopa defend and deflect a bit 
	if (e->ny < 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		if (koopa->GetState() == KOOPA_STATE_WALKING|| koopa->GetState() == KOOPA_STATE_KICKED)
		{
			
			koopa->SetState(KOOPA_STATE_DEFEND);
		}
		else if (koopa->GetState() == KOOPA_STATE_DEFEND)
		{
			
			koopa->SetIsHeld(false);
			koopa->SetState(KOOPA_STATE_KICKED);
		}
		

	}
	else
	{
		if (isAttacking)
		{
			koopa->SetNX(-e->nx);
			koopa->SetState(KOOPA_STATE_UP);
			
		}
		else {
			if (untouchable == 0)
			{
				if (koopa->GetState() == KOOPA_STATE_DEFEND)
				{
					if (canHold == true) {
						koopa->SetIsHeld(true);
						isHolding = true;
					}
					else
					{
						Kick();
						koopa->SetIsHeld(false);
						koopa->SetNX(-e->nx);
						koopa->SetState(KOOPA_STATE_KICKED);

					}
				}
				else
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						SetLevel(level - 1);
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(MARIO_STATE_DIE);
					}
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}


void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	float x, y;
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
	questionBrick->GetPosition(x, y);
	// jump from bottom
	if (e->ny > 0||isAttacking)
	{
		if (!questionBrick->GetIsEmpty())
		{
			questionBrick->SetState(QUESTION_BRICK_STATE_UP);
			if (questionBrick->GetType() == QUESTION_BRICK_TYPE_COIN)
			{
				coin++;
				CCoin* coin = new CCoin(x, y - QUESTION_BRICK_BBOX_HEIGHT);
				coin->SetSpeed(0, -0.5f);
				scene->AddObject(coin);
			}
			else if (questionBrick->GetType() == QUESTION_BRICK_TYPE_ITEM)
			{
				if (level == MARIO_LEVEL_SMALL)
				{
					CMushRoom* mushroom = new CMushRoom(x, y, MUSHROOM_TYPE_RED);
					mushroom->SetState(MUSHROOM_STATE_UP);
					scene->AddObject(mushroom);
				}
				else
				{
					CLeaf* leaf = new CLeaf(x, y);
					leaf->SetState(LEAF_STATE_UP);
					scene->AddObject(leaf);
				}
			}
		}
		
	}
}

void CMario::OnCollisionWithSpecialBrick(LPCOLLISIONEVENT e)
{
	float x, y;
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CSpecialBrick* specialBrick = dynamic_cast<CSpecialBrick*>(e->obj);
	specialBrick->GetPosition(x, y);
	// jump from bottom
	if (e->ny > 0)
	{
		if (!specialBrick->GetIsEmpty())
		{
			specialBrick->SetState(QUESTION_BRICK_STATE_UP);
			if (specialBrick->GetType() == SPECIAL_BRICK_TYPE_MUSHROOM)
			{
				CMushRoom* mushroom = new CMushRoom(x, y, MUSHROOM_TYPE_GREEN);
				mushroom->SetState(MUSHROOM_STATE_UP);
				scene->AddObject(mushroom);
			}
			if (specialBrick->GetType() == SPECIAL_BRICK_TYPE_NORMAL)
			{
				specialBrick->Delete();
			}
		}
	}
}

void CMario::OnCollisionWithMushRoom(LPCOLLISIONEVENT e)
{
	if (level <= MARIO_LEVEL_BIG)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
		SetLevel(level + 1);
	}
	StartUntouchable();
	e->obj->Delete();
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	if (level < MARIO_LEVEL_TAIL)
		SetLevel(level+1);
	StartUntouchable();
	e->obj->Delete();
}

void CMario::OnCollisionWithSpecialPlatform(LPCOLLISIONEVENT e)
{
	CSpecialPlatform* specialPlat = dynamic_cast<CSpecialPlatform*>(e->obj);
	float xPlat, yPlat;
	specialPlat->GetPosition(xPlat, yPlat);
	//block if fall down
	if (e->ny < 0) {
		if (level == MARIO_LEVEL_SMALL) {

			if (yPlat - y < (MARIO_SMALL_BBOX_HEIGHT + 4))
			{
				SetPosition(x,yPlat - MARIO_SMALL_BBOX_HEIGHT - 2);
				vy = 0;
				isOnPlatform = true;
			}
		}
		else {
			if (!isSitting) {
				if (yPlat - y < MARIO_BIG_BBOX_HEIGHT)
				{
					SetPosition(x, yPlat - MARIO_BIG_BBOX_HEIGHT + 4);
					vy = 0;
					isOnPlatform = true;
				}
			}
			else {
				if (yPlat- y < MARIO_BIG_BBOX_HEIGHT / 2 + 4)
				{
					SetPosition(x, yPlat - MARIO_BIG_BBOX_HEIGHT / 2 - 4);
					vy = 0;
					isOnPlatform = true;
				}
			}
		}
	}
}


void CMario::OnCollisionWithFireVenusTrap(LPCOLLISIONEVENT e)
{
	if (isAttacking)
	{
		e->obj->SetState(FIREVENUSTRAP_STATE_DIE);
	}
	if (untouchable == 0)
	{
		if (e->obj->GetState() != FIREVENUSTRAP_STATE_HIDE&& e->obj->GetState() != FIREVENUSTRAP_STATE_DIE)
		{
			if (level > MARIO_LEVEL_SMALL)
			{
				SetLevel(level - 1);
				StartUntouchable();
			}
			else
			{
				DebugOut(L">>> Mario DIE >>> \n");
				SetState(MARIO_STATE_DIE);
			}
		}
	}
}

void CMario::OnCollisionWithFireBall(LPCOLLISIONEVENT e)
{
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			SetLevel(level - 1);
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}


void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isTransforming)
	{
		if (!isKicking)
		{
			if (!isHolding)
			{
				if (!isOnPlatform)
				{
					if (runLevel == 2&&abs(ax)==MARIO_ACCEL_RUN_X)
					{
						if (nx >= 0)
							aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
						else
							aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
					}
					else
					{
						if (nx >= 0)
							aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
						else
							aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
					}
				}
				else
					if (isSitting)
					{
						if (nx > 0)
							aniId = ID_ANI_MARIO_SIT_RIGHT;
						else
							aniId = ID_ANI_MARIO_SIT_LEFT;
					}
					else
						if (vx == 0)
						{
							if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
							else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
						}
						else if (vx > 0)
						{
							if (ax < 0)
								aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
							else if (ax == MARIO_ACCEL_RUN_X && runLevel == 2)
								aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
							else
								aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
						}
						else // vx < 0
						{
							if (ax > 0)
								aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
							else if (ax == -MARIO_ACCEL_RUN_X && runLevel == 2)
								aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
							else
								aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
						}

			}
			else
			{
				if (!isOnPlatform)
				{
					if (nx >= 0)
						aniId = ID_ANI_MARIO_SMALL_HOLDING_JUMP_RIGHT;
					else
						aniId = ID_ANI_MARIO_SMALL_HOLDING_JUMP_LEFT;

				}
				else

					if (vx == 0)
					{
						if (nx > 0) aniId = ID_ANI_MARIO_SMALL_HOLDING_RIGHT;
						else aniId = ID_ANI_MARIO_SMALL_HOLDING_LEFT;
					}
					else if (vx > 0)
					{
						aniId = ID_ANI_MARIO_SMALL_HOLDING_RUN_RIGHT;
					}
					else // vx < 0
					{
						aniId = ID_ANI_MARIO_SMALL_HOLDING_RUN_LEFT;
					}
			}
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_KICKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_KICKING_LEFT;
		}
	}
	else
	{
		if (nx >= 0)
		{
			aniId = ID_ANI_MARIO_FROM_SMALL_TO_BIG_LEFT;
		}
		else
		{
			aniId = ID_ANI_MARIO_FROM_SMALL_TO_BIG_LEFT;
		}

	}
	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isTransforming)
	{
		if (!isKicking)
		{
			if (!isHolding)
			{
				if (!isOnPlatform)
				{
					if (runLevel == 2 && abs(ax) == MARIO_ACCEL_RUN_X)
					{
						if (nx >= 0)
							aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
						else
							aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
					}
					else
					{
						if (nx >= 0)
							aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
						else
							aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
					}
				}
				else
					if (isSitting)
					{
						if (nx > 0)
							aniId = ID_ANI_MARIO_SIT_RIGHT;
						else
							aniId = ID_ANI_MARIO_SIT_LEFT;
					}
					else
						if (vx == 0)
						{
							if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
							else aniId = ID_ANI_MARIO_IDLE_LEFT;
						}
						else if (vx > 0)
						{
							if (ax < 0)
								aniId = ID_ANI_MARIO_BRACE_RIGHT;
							else if (ax == MARIO_ACCEL_RUN_X && runLevel == 2)
								aniId = ID_ANI_MARIO_RUNNING_RIGHT;
							else 
								aniId = ID_ANI_MARIO_WALKING_RIGHT;
						}
						else // vx < 0
						{
							if (ax > 0)
								aniId = ID_ANI_MARIO_BRACE_LEFT;
							else if (ax == -MARIO_ACCEL_RUN_X && runLevel == 2)
								aniId = ID_ANI_MARIO_RUNNING_LEFT;
							else
								aniId = ID_ANI_MARIO_WALKING_LEFT;
						}
			}
			else
			{
				if (!isOnPlatform)
				{
					if (nx >= 0)
						aniId = ID_ANI_MARIO_HOLDING_JUMP_RIGHT;
					else
						aniId = ID_ANI_MARIO_HOLDING_JUMP_LEFT;

				}
				else

					if (vx == 0)
					{
						if (nx > 0) aniId = ID_ANI_MARIO_HOLDING_RIGHT;
						else aniId = ID_ANI_MARIO_HOLDING_LEFT;
					}
					else if (vx > 0)
					{
						aniId = ID_ANI_MARIO_HOLDING_RUN_RIGHT;
					}
					else // vx < 0
					{
						aniId = ID_ANI_MARIO_HOLDING_RUN_LEFT;
					}
			}
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_KICKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_KICKING_LEFT;
		}
	}
	else
	{
		if (nx >= 0)
		{
			aniId = ID_ANI_MARIO_FROM_SMALL_TO_BIG_RIGHT;
		}
		else
		{
			aniId = ID_ANI_MARIO_FROM_SMALL_TO_BIG_LEFT;
		}
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}


int CMario::GetAniIdTail()
{
	int aniId = -1;
	if (!isTransforming) {
		if (!isKicking)
		{
			if (!isHolding)
			{
				if (isAttacking)
				{
					if (nx > 0)
						aniId = ID_ANI_MARIO_TAIL_ATTACK_RIGHT;
					else
						aniId = ID_ANI_MARIO_TAIL_ATTACK_LEFT;
				}
				else
				{
					if (!isOnPlatform)
					{
						if (runLevel == 2 && abs(ax) == MARIO_ACCEL_RUN_X)
						{
							if (flyJump)
							{
								if (nx >= 0)
									aniId = ID_ANI_MARIO_TAIL_FLYING_JUMP_RIGHT;
								else
									aniId = ID_ANI_MARIO_TAIL_FLYING_JUMP_LEFT;
							}
							else
							{
								if (nx >= 0)
									aniId = ID_ANI_MARIO_TAIL_JUMP_RUN_RIGHT;
								else
									aniId = ID_ANI_MARIO_TAIL_JUMP_RUN_LEFT;
							}
						}
						else
						{
							if (flyJump && isFlying)
							{
								if (nx >= 0)
									aniId = ID_ANI_MARIO_TAIL_FLYING_DROP_JUMP_RIGHT;
								else
									aniId = ID_ANI_MARIO_TAIL_FLYING_DROP_JUMP_LEFT;
							}
							else if (isFlying)
							{
								if (nx >= 0)
									aniId = ID_ANI_MARIO_TAIL_FLYING_DROP_RIGHT;
								else
									aniId = ID_ANI_MARIO_TAIL_FLYING_DROP_LEFT;
							}
							else
							{
								if (nx >= 0)
									aniId = ID_ANI_MARIO_TAIL_JUMP_WALK_RIGHT;
								else
									aniId = ID_ANI_MARIO_TAIL_JUMP_WALK_LEFT;
							}
						}
					}
					else
						if (isSitting)
						{
							if (nx > 0)
								aniId = ID_ANI_MARIO_TAIL_SIT_RIGHT;
							else
								aniId = ID_ANI_MARIO_TAIL_SIT_LEFT;
						}
						else
							if (vx == 0)
							{
								if (nx > 0) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;
								else aniId = ID_ANI_MARIO_TAIL_IDLE_LEFT;
							}
							else if (vx > 0)
							{
								if (ax < 0)
									aniId = ID_ANI_MARIO_TAIL_BRACE_RIGHT;
								else if (ax == MARIO_ACCEL_RUN_X && runLevel == 2)
									aniId = ID_ANI_MARIO_TAIL_RUNNING_RIGHT;
								else
									aniId = ID_ANI_MARIO_TAIL_WALKING_RIGHT;
							}
							else // vx < 0
							{
								if (ax > 0)
									aniId = ID_ANI_MARIO_TAIL_BRACE_LEFT;
								else if (ax == -MARIO_ACCEL_RUN_X && runLevel == 2)
									aniId = ID_ANI_MARIO_TAIL_RUNNING_LEFT;
								else
									aniId = ID_ANI_MARIO_TAIL_WALKING_LEFT;
							}
				}
			}
			else
			{
				if (!isOnPlatform)
				{
					if (nx >= 0)
						aniId = ID_ANI_MARIO_TAIL_HOLDING_JUMP_RIGHT;
					else
						aniId = ID_ANI_MARIO_TAIL_HOLDING_JUMP_LEFT;

				}
				else

					if (vx == 0)
					{
						if (nx > 0) aniId = ID_ANI_MARIO_TAIL_HOLDING_RIGHT;
						else aniId = ID_ANI_MARIO_TAIL_HOLDING_LEFT;
					}
					else if (vx > 0)
					{
						aniId = ID_ANI_MARIO_TAIL_HOLDING_RUN_RIGHT;
					}
					else // vx < 0
					{
						aniId = ID_ANI_MARIO_TAIL_HOLDING_RUN_LEFT;
					}
			}
		}
		else
		{

			if (nx >= 0)
				aniId = ID_ANI_MARIO_TAIL_KICKING_RIGHT;
			else
				aniId = ID_ANI_MARIO_TAIL_KICKING_LEFT;
		}
	}
	else
	{
		if (nx >= 0)
		{
			aniId = ID_ANI_MARIO_FROM_BIG_TO_TAIL_RIGHT;
		}
		else
		{
			aniId = ID_ANI_MARIO_FROM_BIG_TO_TAIL_LEFT;
		}
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_TAIL_IDLE_RIGHT;

	return aniId;

}


void CMario::Render()
{
	
	int aniId = -1;

	
	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_TAIL)
		aniId = GetAniIdTail();
	if (aniId == -1) return;
	CAnimations* animations = CAnimations::GetInstance();
		animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}


void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting||vy>0) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting||vy>0) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		else
		{
			if (GetLevel() == MARIO_LEVEL_TAIL)
			{
				Fly();
			}

		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 4;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::Fly()
{
	if (runLevel == 2 && abs(ax) == MARIO_ACCEL_RUN_X) {
		vy = MARIO_JUMP_FLY_SPEED_Y;
		ay = 0;
		flyJump_start = GetTickCount64();
		isFlying = true;
		flyJump = true;
	}
	if (vy > 0&&isFlying)
	{
		coin++;
		vy = 0;
		ay = MARIO_GRAVITY / 10;
		flyJump_start = GetTickCount64();	
		isFlying = true;
		flyJump = true;
	}
}

void CMario::TailAttack()
{
	isAttacking = true;
	tailAttack_start = GetTickCount64();
}


void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	
	if (level==MARIO_LEVEL_SMALL)
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
	else
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else if (isAttacking)
		{
			left = x - (MARIO_BIG_BBOX_WIDTH+TAIL_LENGTH) / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + (MARIO_BIG_BBOX_WIDTH + TAIL_LENGTH);
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	isTransforming = true;
	level = l;
	
}

