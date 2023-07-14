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
#include"Button.h"
#include "Pipe.h"
#include "Card.h"
#include "BackgroundTile.h"
#include "Node.h"
#include "Path.h"
#include "Effect.h"
#include "Data.h"

#include "Collision.h"

CMario::CMario(float x, float y):CGameObject(x,y)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (scene->GetId() == ID_SCENE_WORLDMAP)
		SetIsInWorldMap(true);
	else
		SetIsInWorldMap(false);
	xD = x;
	yD = y;
	isSitting = false;
	maxVx = 0.0f;
	ax = 0.0f;
	ay = MARIO_GRAVITY;
	nx = 1;
	SetLevel(CData::GetInstance()->marioLevel);
	untouchable = 0;
	untouchable_start = -1;
	usingPipe_start = -1;
	isOnPlatform = false;
	canHold = false;
	isKicking = false;
	isTransforming = false;
	coin = 0;
	runLevel = 0;
	state = MARIO_STATE_IDLE;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CData::GetInstance()->marioRunLevel = runLevel;
	//update timer
	if (!isDead&&GetTickCount64() - second_start > 1000&&!isInWorldMap)
		UpdateTimer();
	
	///time out
	if (CData::GetInstance()->timer < 0) SetState(MARIO_STATE_DIE);

	//annouce course clear
	if (isEndScene && GetTickCount64() - endScene_start > MARIO_END_SCENE_TIME)
	{
		EndScene();
		isEndScene = false;
		prepareScene_start = GetTickCount64();
		isSwitchScene = true;
		entrance_id = ID_SCENE_WORLDMAP;
	}

	///mario dead
	if (isDead && GetTickCount64() - die_start > MARIO_DIE_TIMEOUT)
	{
		isDead = false;
		CData::GetInstance()->timer = LIMIT_TIME;
		entrance_id = ID_SCENE_WORLDMAP;
		CGame::GetInstance()->InitiateSwitchScene(entrance_id);
	}

	//back to worldmap
	if (GetTickCount64() - prepareScene_start> MARIO_END_SCENE_TIME/2 && isSwitchScene)
	{
		isSwitchScene = false;
		CGame::GetInstance()->InitiateSwitchScene(entrance_id);
	}

	///set mario can fly 
	if (runLevel == 2 && abs(vx) == MARIO_RUNNING_SPEED&&isOnPlatform) 
	{ 
		canFly = true; 
		canFly_start = GetTickCount64();
	}
	// reset untouchable timer if untouchable time has passed
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

	////using pipe action
	if (GetTickCount64() - usingPipe_start >= MARIO_USING_PIPE_TIME/2 &&isUsingPipe)
	{
		
		if (GetTickCount64() - usingPipe_start == MARIO_USING_PIPE_TIME/2) SetPosition(xD, yD);
		if (GetTickCount64() - usingPipe_start >= MARIO_USING_PIPE_TIME)
		{
			isUsingPipe = false;
			SetState(MARIO_STATE_IDLE);
		}
			
		
	}

	if (GetTickCount64() - tailAttack_start >= MARIO_TAIL_ATTACK_TIME)
	{
		isAttacking = false;
	}
	if (GetTickCount64() - flyJump_start >= MARIO_FLY_JUMP_TIME&&flyJump)
	{
	
		flyJump = false;
		ay = MARIO_GRAVITY;
	}
	if (GetTickCount64() - canFly_start >= MARIO_FLY_CAN_FLY_TIME)
	{
		canFly = false;
	}
	if (isOnPlatform) { isFlying = false; ay = 0.002f; }
	else
	{
		ay = MARIO_GRAVITY;
	}
	if (isTransforming) vx = vy = 0;
	else
	{
		if (state != MARIO_STATE_DOWN_PIPE&&state!=MARIO_STATE_UP_PIPE&&!isInWorldMap)
			vy += ay * dt;
		vx += ax * dt;
	}
	if (isFlying) ax = nx*MARIO_ACCEL_RUN_X;

	if (abs(vx) >= abs(maxVx)) { vx = maxVx; SetRunLevel(2); }
	else
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X) SetRunLevel(1);
		else
			SetRunLevel(0);
	}
	if (!isInWorldMap)
	{////begin and end point of scene
		if (x <= MARIO_SMALL_BBOX_WIDTH / 2)x = MARIO_SMALL_BBOX_WIDTH / 2;
		if (x >= MAP_WIDTH - MARIO_SMALL_BBOX_WIDTH / 2 && x <= MAP_WIDTH + MARIO_SMALL_BBOX_WIDTH / 2 && !isEndScene)x = MAP_WIDTH - MARIO_SMALL_BBOX_WIDTH / 2;

		///drop from floor and die
		if (y >= HEIGHT_DEATH && y <= HEIGHT_DEATH + MARIO_BIG_BBOX_HEIGHT) { SetState(MARIO_STATE_DIE); }
		else if (y <= 0) {
			canFly = false;
			y = MARIO_BIG_BBOX_HEIGHT / 2;
			vy = 0.0001f;
			if (nx > 0) SetState(MARIO_STATE_WALKING_RIGHT);
			else
				SetState(MARIO_STATE_WALKING_LEFT);
		}
	}
	
	
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
	else if (dynamic_cast<CPipe*>(e->obj))
	{
		OnCollisionWithPipe(e);
	}
	else if (dynamic_cast<CCard*>(e->obj))
		OnCollisionWithCard(e);
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
	else if (dynamic_cast<CButton*>(e->obj))
		OnCollisionWithButton(e);
	else if (dynamic_cast<CNode*>(e->obj))
		OnCollisionWithNode(e);

	
}

void CMario::EndScene()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	CBackgroundTile* announce1 = new CBackgroundTile(ANNOUCE_X, ANNOUCE_Y, ID_SPRITE_COURSE_CLEAR);
	CBackgroundTile* announce2 = new CBackgroundTile(ANNOUCE_X, ANNOUCE_Y+24, ID_SPRITE_YOU_GOT_A_CARD);
	scene->AddObject(announce1);
	scene->AddObject(announce2);
	scene->AddObject(this->card);

	
	
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
			CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
			AddScore(100);
		}

	}
	else // hit by Goomba
	{
		if (isAttacking)
		{
			goomba->SetState(GOOMBA_STATE_JUMP_DIE);
			goomba->SetSpeed(nx*MARIO_TAIL_ATTACK_SPEED_X, MARIO_TAIL_ATTACK_SPEED_Y);
			CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
			AddScore(100);
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
	if (koopa->GetState() == KOOPA_STATE_DIE) return;
	// jump on top >> Koopa defend and deflect a bit 
	if (e->ny < 0)
	{
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		if (koopa->GetLevel() == KOOPA_LEVEL_NORMAL)
		{
			if (koopa->GetState() == KOOPA_STATE_WALKING || koopa->GetState() == KOOPA_STATE_KICKED)
			{
				CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
				AddScore(100);
				koopa->SetState(KOOPA_STATE_DEFEND);
			}
			else if (koopa->GetState() == KOOPA_STATE_DEFEND)
			{
				CEffect* effect = new CEffect(x, y, EFFECT_SCORE_200);
				AddScore(200);
				koopa->SetIsHeld(false);
				koopa->SetState(KOOPA_STATE_KICKED);

			}
		}
		else
		{
			CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
			koopa->SetLevel(KOOPA_LEVEL_NORMAL);
		}

	}
	else
	{
		if (isAttacking)
		{
			CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
			AddScore(100);
			koopa->SetNX(-e->nx);
			koopa->SetState(KOOPA_STATE_UP);
			koopa->SetLevel(KOOPA_LEVEL_NORMAL);
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
	AddCoin();
}


void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	float x, y;
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
	questionBrick->GetPosition(x, y);
	// jump from bottom
	if (e->ny > 0 || isAttacking)
	{
		if (!questionBrick->GetIsEmpty())
		{
			questionBrick->SetState(QUESTION_BRICK_STATE_UP);
			if (questionBrick->GetType() == QUESTION_BRICK_TYPE_COIN)
			{
				AddCoin();
				AddScore(100);
				CCoin* coin = new CCoin(x, y - QUESTION_BRICK_BBOX_HEIGHT);
				coin->SetState(COIN_STATE_OUT);
				scene->AddObject(coin);
				CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
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
	if (e->ny > 0||isAttacking)
	{
		if (!specialBrick->GetIsEmpty())
		{
			specialBrick->SetState(QUESTION_BRICK_STATE_UP);
			if (specialBrick->GetType() == SPECIAL_BRICK_TYPE_NORMAL)
			{
				specialBrick->Break();
				specialBrick->Delete();

			}
			if (specialBrick->GetType() == SPECIAL_BRICK_TYPE_MUSHROOM)
			{
				CMushRoom* mushroom = new CMushRoom(x, y, MUSHROOM_TYPE_GREEN);
				mushroom->SetState(MUSHROOM_STATE_UP);
				scene->AddObject(mushroom);
			}
			if (specialBrick->GetType() == SPECIAL_BRICK_TYPE_BUTTON)
			{
				CButton* button = new CButton(x, y - BUTTON_BBOX_HEIGHT);
				scene->AddObject(button);
			}
			
		}
	}
}

void CMario::OnCollisionWithMushRoom(LPCOLLISIONEVENT e)
{
	CMushRoom* mushroom = dynamic_cast<CMushRoom*>(e->obj);
	if (mushroom->GetType() == MUSHROOM_TYPE_RED)
	{
		if (level <= MARIO_LEVEL_BIG)
		{
			y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
			SetLevel(level + 1);
		}
		CEffect* effect = new CEffect(x, y, EFFECT_SCORE_1000);
		AddScore(1000);
		StartUntouchable();
		
	}
	else
	{
		
		AddHP(1);
		CEffect* effect_ = new CEffect(x, y, EFFECT_SCORE_1UP);
	}
	e->obj->Delete();
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CEffect* effect = new CEffect(x, y, EFFECT_SCORE_1000);
	AddScore(1000);
	if (level < MARIO_LEVEL_TAIL)
		SetLevel(level+1);
	StartUntouchable();
	e->obj->Delete();
}

void CMario::OnCollisionWithButton(LPCOLLISIONEVENT e)
{
	CButton* button = dynamic_cast<CButton*>(e->obj);
	button->SetState(BUTTON_STATE_CLICKED);

	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<CCoin*> coins;
	for (long i = 0; i < scene->GetObjectsQuanity(); i++)
	{
		if (dynamic_cast<CSpecialBrick*>(scene->GetObject(i)))
		{
			CSpecialBrick* brick = dynamic_cast<CSpecialBrick*>(scene->GetObject(i));
			if (brick->GetType() == SPECIAL_BRICK_TYPE_NORMAL)
			{
				float xC, yC;
				brick->GetPosition(xC, yC);
				CCoin* coin = new CCoin(xC, yC);
				coins.push_back(coin);
				brick->Delete();
			}
		}
	}
	for (int i = 0; i < coins.size(); i++)
	{
		scene->AddObject(coins[i]);
	}
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
		CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
		AddScore(100);
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

void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e)
{
	isOnPipe = true;
	if (e->ny != 0)
	{
		if (e->ny < 0 && canGoDownPipe || e->ny>0 && canGoUpPipe)
			UsingPipe(e);
		else
		{
			vy = 0;
			if (e->ny < 0) isOnPlatform = true;
		}
	}
	if (e->nx != 0) 
		vx = 0;
}

void CMario::UsingPipe(LPCOLLISIONEVENT e)
{
	CPipe* pipe = dynamic_cast<CPipe*>(e->obj);
	
	if (pipe->GetIsCanGo())
	{
		xD = pipe->GetDestinationX();
		yD = pipe->GetDestinationY();
		if(canGoDownPipe)
			SetState(MARIO_STATE_DOWN_PIPE);
		if (canGoUpPipe)
			SetState(MARIO_STATE_UP_PIPE);
	}

}

void CMario::OnCollisionWithCard(LPCOLLISIONEVENT e)
{
	CCard* card = dynamic_cast<CCard*>(e->obj);
	this->card = new CCard(ANNOUCE_X+61, ANNOUCE_Y+24, card->GetType(), CARD_STATE_HUD);
	card->SetState(CARD_STATE_UP);
	isEndScene = true;
	endScene_start = GetTickCount64();
	SetState(MARIO_STATE_WALKING_RIGHT);
	CData::GetInstance()->SetCard(this->card->GetType());

}

void CMario::OnCollisionWithNode(LPCOLLISIONEVENT e)
{
	CNode* node = dynamic_cast<CNode*>(e->obj);
	node->GetPosition(xNode, yNode);
	SetState(MARIO_STATE_WORLDMAP_IDLE);
	SetPosition(xNode, yNode);
	this->entrance_id = node->GetSceneId();
	this->canEnterNode = node->IsCanPlay();
	this->canMoveDown = node->canMoveDown;
	this->canMoveUp = node->canMoveUp;
	this->canMoveRight = node->canMoveRight;
	this->canMoveLeft = node->canMoveLeft;
	isMoving = false;
}

void CMario::EnterNode()
{
	if (this->canEnterNode)
	{
		CGame::GetInstance()->InitiateSwitchScene(entrance_id);
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
	if (!isInWorldMap)
	{
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
	}
	else
	{
		if (level == MARIO_LEVEL_SMALL)
			aniId = ID_ANI_MARIO_SMALL_WORLDMAP;
		else if (level == MARIO_LEVEL_BIG)
			aniId = ID_ANI_MARIO_WORLDMAP;
		else if (level == MARIO_LEVEL_TAIL)
			aniId = ID_ANI_MARIO_TAIL_WORLDMAP;
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
	if (!isInWorldMap)
	{
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
	}
	else
	{
		aniId = ID_ANI_MARIO_WORLDMAP;
	}
	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}


int CMario::GetAniIdTail()
{
	int aniId = -1;
	if (!isInWorldMap)
	{
		if (!isUsingPipe)
		{
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
								if (canFly)
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
		}
		else
		{
			aniId = ID_ANI_MARIO_TAIL_GO_PIPE;
		}
	}
	else
	{
		aniId = ID_ANI_MARIO_TAIL_WORLDMAP;
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
		AddHP(-1);
		die_start = GetTickCount64();
		isDead = true;
		break;
	case MARIO_STATE_DOWN_PIPE:
		isOnPlatform = false;
		usingPipe_start = GetTickCount64();
		isUsingPipe = true;
		vy = MARIO_GET_IN_PIPE_SPEED;
		ay = 0;
		break;
	case MARIO_STATE_UP_PIPE:
		isOnPlatform = false;
		usingPipe_start = GetTickCount64();
		isUsingPipe = true;
		vy = -MARIO_GET_IN_PIPE_SPEED;
		vx = 0;
		ay = 0;
		break;
	case MARIO_STATE_WORLDMAP_IDLE:
		nx = 0;
		ax = ay = 0;
		vx = vy = 0;
		break;
	case MARIO_STATE_WORLDMAP_WALK_RIGHT:
		isMoving = true;
		nx = 1;
		ny = 0;
		maxVx=vx = MARIO_WALKING_SPEED;
		vy = 0;
		maxVx = vx;
		break;
	case MARIO_STATE_WORLDMAP_WALK_LEFT:
		isMoving = true;
		nx = -1;
		ny = 0;
		maxVx=vx = -MARIO_WALKING_SPEED;
		vy = 0;
		break;
	case MARIO_STATE_WORLDMAP_WALK_UP:
		isMoving = true;
		ny = -1;
		nx = 0;
		vy = -MARIO_WALKING_SPEED;
		vx = 0;
		break;
	case MARIO_STATE_WORLDMAP_WALK_DOWN:
		isMoving = true;
		ny = 1;
		nx = 0;
		vy = MARIO_WALKING_SPEED;
		vx = 0;
		break;

	}


	CGameObject::SetState(state);
}

void CMario::Fly()
{
	if (canFly) 
	{
		ay = -MARIO_GRAVITY;
		if (vy = MARIO_JUMP_FLY_SPEED_Y)
			vy = MARIO_JUMP_FLY_SPEED_Y;
		flyJump_start = GetTickCount64();
		canFly_start = GetTickCount64();
		isFlying = true;
		flyJump = true;
		canFly = true;
	}
	else if (vy > 0)
	{
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
	if (this->level == MARIO_LEVEL_SMALL&&!isInWorldMap)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	isTransforming = true;
	level = l;
	CData::GetInstance()->marioLevel = level;
}

void CMario::AddCoin()
{
	coin++;
	CData::GetInstance()->coin++;
}

void CMario::AddScore(int score)
{
	CData::GetInstance()->score += score;
}

void CMario::AddHP(int HP)
{
	CData::GetInstance()->HP += HP;
}

void CMario::UpdateTimer()
{
	second_start = GetTickCount64();
	CData::GetInstance()->timer--;
}

