#pragma once
#include "GameObject.h"

#define ID_ANI_RED_FIREVENUSTRAP_LEFT_DOWN 15101
#define ID_ANI_RED_FIREVENUSTRAP_LEFT_UP 15102

#define ID_ANI_RED_FIREVENUSTRAP_RIGHT_DOWN 15111
#define ID_ANI_RED_FIREVENUSTRAP_RIGHT_UP 15112

#define ID_ANI_GREEN_FIREVENUSTRAP_LEFT_DOWN 15103
#define ID_ANI_GREEN_FIREVENUSTRAP_LEFT_UP 15104

#define ID_ANI_GREEN_FIREVENUSTRAP_RIGHT_DOWN 15113
#define ID_ANI_GREEN_FIREVENUSTRAP_RIGHT_UP 15114




#define FIREVENUSTRAP_BBOX_WIDTH 16
#define FIREVENUSTRAP_RED_BBOX_HEIGHT 32
#define FIREVENUSTRAP_GREEN_BBOX_HEIGHT 24

#define FIREVENUSTRAP_SPEED 0.02f
#define FIREVENUSTRAP_FIREBALL_SPEED 0.005f
#define FIREVENUSTRAP_RANGE_MAX 128
#define FIREVENUSTRAP_RANGE_MIN 32

#define FIREVENUSTRAP_STATE_HIDE 100
#define FIREVENUSTRAP_STATE_UP 200
#define FIREVENUSTRAP_STATE_DOWN 300
#define FIREVENUSTRAP_STATE_ATTACK 400
#define FIREVENUSTRAP_STATE_DIE 500

#define FIREVENUSTRAP_TIME_ATTACK 1000
#define FIREVENUSTRAP_DIE_TIME_OUT 1000

#define FIREVENUSTRAP_TYPE_RED 1
#define FIREVENUSTRAP_TYPE_GREEN 2


class CFireVenusTrap :public CGameObject
{
protected:
	float xStart, yStart, ny;
	ULONGLONG attack_start, attack_gap, die_start;
	int type;

	int GetAniIdRed();
	int GetAniIdGreen();

public:
	CFireVenusTrap(float x, float y, int type) :CGameObject(x, y) { 
		vx = 0; vy = 0; xStart = x; yStart = y; 
		state = FIREVENUSTRAP_STATE_HIDE; 
		ny = 0; attack_start = -1; attack_gap = -1;
		this->type = type;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	
	void OnNoCollision(DWORD dt);
	void SetState(int state);

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
};

