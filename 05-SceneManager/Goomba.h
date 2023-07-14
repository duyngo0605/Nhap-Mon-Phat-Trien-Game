#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.0007f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_FLYING_SPEED -0.25f

#define GOOMBA_TYPE_NORMAL 1
#define GOOMBA_TYPE_PARA 2


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 10
#define PARAGOOMBA_BBOX_WIDTH 20
#define PARAGOOMBA_BBOX_HEIGHT 22

#define GOOMBA_DIE_TIMEOUT 1000
#define GOOMBA_WALK_TIMEOUT 2000
#define GOOMBA_PREPARE_FLY_TIMEOUT 1000
#define GOOMBA_FLY_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_FLYING 300
#define GOOMBA_STATE_PREPARE_FLY 400
#define GOOMBA_STATE_JUMP_DIE 500

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_JUMP_DIE 5002

#define ID_ANI_PARAGOOMBA_WALKING 5100
#define ID_ANI_PARAGOOMBA_WINGS_WALKING 5101
#define ID_ANI_PARAGOOMBA_DIE 5102
#define ID_ANI_PARAGOOMBA_PREPARE_FLY 5103
#define ID_ANI_PARAGOOMBA_FLY_LEFT 5104
#define ID_ANI_PARAGOOMBA_FLY_RIGHT 5105
#define ID_ANI_PARAGOOMBA_TAIL_ATTACKED 5106


class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 
	
	float xStart;
	float yStart;

	int type;
	int level;

	ULONGLONG die_start;
	ULONGLONG walk_start;
	ULONGLONG prepare_fly_start;
	ULONGLONG fly_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithSpecialPlatform(LPCOLLISIONEVENT e);

public: 	
	
	CGoomba(float x, float y, int type);
	int GetLevel() { return this->level; }
	void MinusLevel() { level--; }
	virtual void SetState(int state);
};