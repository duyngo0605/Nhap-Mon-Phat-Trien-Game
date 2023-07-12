#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Card.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.18f

#define MARIO_ACCEL_WALK_X	0.0001f
#define MARIO_ACCEL_RUN_X	0.00015f

#define MARIO_JUMP_SPEED_Y		0.4f
#define MARIO_JUMP_RUN_SPEED_Y	0.5f
#define MARIO_JUMP_FLY_SPEED_Y	-0.2f
#define MARIO_GET_IN_PIPE_SPEED	0.05f

#define MARIO_GRAVITY			0.001f
#define MARIO_ACCEL_JUMP_Y		0.00005f

#define MARIO_JUMP_DEFLECT_SPEED  0.3f

#define MARIO_TAIL_ATTACK_SPEED_X 0.05f
#define MARIO_TAIL_ATTACK_SPEED_Y -0.3f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_FLY				800
#define MARIO_STATE_FLY_RELEASE		801

#define MARIO_STATE_DOWN_PIPE		900
#define MARIO_STATE_UP_PIPE			901


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_KICKING_RIGHT 100
#define ID_ANI_MARIO_KICKING_LEFT 101

#define ID_ANI_MARIO_HOLDING_RIGHT 200
#define ID_ANI_MARIO_HOLDING_LEFT 201

#define ID_ANI_MARIO_HOLDING_RUN_RIGHT 300
#define ID_ANI_MARIO_HOLDING_RUN_LEFT 301

#define ID_ANI_MARIO_HOLDING_JUMP_RIGHT 310
#define ID_ANI_MARIO_HOLDING_JUMP_LEFT 311

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_HOLDING_RIGHT 1700
#define ID_ANI_MARIO_SMALL_HOLDING_LEFT 1701

#define ID_ANI_MARIO_SMALL_HOLDING_RUN_RIGHT 1800
#define ID_ANI_MARIO_SMALL_HOLDING_RUN_LEFT 1801

#define ID_ANI_MARIO_SMALL_HOLDING_JUMP_RIGHT 1810
#define ID_ANI_MARIO_SMALL_HOLDING_JUMP_LEFT 1801

#define ID_ANI_MARIO_SMALL_KICKING_RIGHT 1900
#define ID_ANI_MARIO_SMALL_KICKING_LEFT 1901


///TAIL MARIO
#define ID_ANI_MARIO_TAIL_IDLE_RIGHT 2100
#define ID_ANI_MARIO_TAIL_IDLE_LEFT 2101

#define ID_ANI_MARIO_TAIL_WALKING_RIGHT 2200
#define ID_ANI_MARIO_TAIL_WALKING_LEFT 2201

#define ID_ANI_MARIO_TAIL_RUNNING_RIGHT 2300
#define ID_ANI_MARIO_TAIL_RUNNING_LEFT 2301

#define ID_ANI_MARIO_TAIL_BRACE_RIGHT 2400
#define ID_ANI_MARIO_TAIL_BRACE_LEFT 2401

#define ID_ANI_MARIO_TAIL_JUMP_WALK_RIGHT 2500
#define ID_ANI_MARIO_TAIL_JUMP_WALK_LEFT 2501

#define ID_ANI_MARIO_TAIL_JUMP_RUN_RIGHT 2600
#define ID_ANI_MARIO_TAIL_JUMP_RUN_LEFT 2601

#define ID_ANI_MARIO_TAIL_SIT_RIGHT 2700
#define ID_ANI_MARIO_TAIL_SIT_LEFT 2701

#define ID_ANI_MARIO_TAIL_HOLDING_RIGHT 2800
#define ID_ANI_MARIO_TAIL_HOLDING_LEFT 2801

#define ID_ANI_MARIO_TAIL_HOLDING_RUN_RIGHT 2900
#define ID_ANI_MARIO_TAIL_HOLDING_RUN_LEFT 2901


#define ID_ANI_MARIO_TAIL_HOLDING_JUMP_RIGHT 2910
#define ID_ANI_MARIO_TAIL_HOLDING_JUMP_LEFT 2911

#define ID_ANI_MARIO_TAIL_KICKING_RIGHT 3000
#define ID_ANI_MARIO_TAIL_KICKING_LEFT 3001

#define ID_ANI_MARIO_TAIL_FLYING_JUMP_RIGHT 3100
#define ID_ANI_MARIO_TAIL_FLYING_JUMP_LEFT 3101

#define ID_ANI_MARIO_TAIL_FLYING_DROP_JUMP_RIGHT 3200
#define ID_ANI_MARIO_TAIL_FLYING_DROP_JUMP_LEFT 3201

#define ID_ANI_MARIO_TAIL_FLYING_DROP_RIGHT 3300
#define ID_ANI_MARIO_TAIL_FLYING_DROP_LEFT 3301

#define ID_ANI_MARIO_TAIL_ATTACK_RIGHT 3400
#define ID_ANI_MARIO_TAIL_ATTACK_LEFT 3401

#define ID_ANI_MARIO_TAIL_GO_PIPE	3500

////Transform animations
#define ID_ANI_MARIO_FROM_SMALL_TO_BIG_RIGHT 4000
#define ID_ANI_MARIO_FROM_BIG_TO_TAIL_RIGHT 4001

#define ID_ANI_MARIO_FROM_SMALL_TO_BIG_LEFT 4100
#define ID_ANI_MARIO_FROM_BIG_TO_TAIL_LEFT 4101






#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define TAIL_LENGTH 4


#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2000
#define MARIO_KICKING_TIME		200
#define MARIO_TAIL_ATTACK_TIME	300
#define MARIO_FLY_JUMP_TIME		100
#define MARIO_FLY_CAN_FLY_TIME	500
#define MARIO_USING_PIPE_TIME	1500
#define MARIO_END_SCENE_TIME	3000
#define HEIGHT_DEATH	500

#define ID_SPRITE_COURSE_CLEAR			11024
#define ID_SPRITE_YOU_GOT_A_CARD		11025

#define ANNOUCE_X	2688
#define	ANNOUCE_Y	256

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	float xD, yD;

	int level;
	int runLevel;

	int untouchable; 
	ULONGLONG untouchable_start;
	ULONGLONG kick_start;
	ULONGLONG flyJump_start;
	ULONGLONG tailAttack_start;
	ULONGLONG canFly_start;
	ULONGLONG usingPipe_start;
	ULONGLONG endScene_start;
	BOOLEAN isOnPlatform;
	int coin; 

	bool canHold;
	bool isHolding = false;
	bool isKicking;
	bool isTransforming;
	bool isFlying = false;
	bool flyJump = false;
	bool isAttacking = false;
	bool canFly = false;
	bool canGoDownPipe = false;
	bool canGoUpPipe = false;
	bool isUsingPipe = false;
	bool isOnPipe = false;
	bool isEndScene = false;

	CCard* card;

	void EndScene();

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithSpecialBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushRoom(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithButton(LPCOLLISIONEVENT e);
	void OnCollisionWithSpecialPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithFireVenusTrap(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBall(LPCOLLISIONEVENT e);
	void OnCollisionWithPipe(LPCOLLISIONEVENT e);
	void UsingPipe(LPCOLLISIONEVENT e);

	void OnCollisionWithCard(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdTail();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		xD = x;
		yD = y;
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 
		nx = 1;
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		usingPipe_start = -1;
		isOnPlatform = false;
		canHold = false;
		isKicking = false;
		isTransforming = false;
		coin = 0;
		runLevel = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void SetRunLevel(int l) { 
		if (l == 2)
		{
			if (!isOnPlatform) return;
		}
		runLevel = l; }
	int GetRunLevel() { return runLevel; }
	bool GetIsHolding() { return isHolding; }
	void SetCanHold(bool canHold) { this->canHold = canHold; if (canHold == false) isHolding = false; }
	void Kick() { isKicking = true; kick_start = GetTickCount64(); }
	bool GetIsTransforming() { return isTransforming; }
	int GetNX() { return nx; }

	void Fly();
	void TailAttack();
	bool GetIsFlying() { return isFlying; }
	void SetCanDownPipe(bool canDown) { this->canGoDownPipe = canDown; }
	void SetCanUpPipe(bool canUp) { this->canGoUpPipe = canUp; }
	bool GetIsUsingPipe() { return isUsingPipe; }
	bool GetIsOnPipe() { return isOnPipe; }

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE && state!=MARIO_STATE_DOWN_PIPE && state!=MARIO_STATE_UP_PIPE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	int GetLevel() { return level; }
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void AddCoin() { coin++; }

	bool GetIsEndScene() { return isEndScene; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};