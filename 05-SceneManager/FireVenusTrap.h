#pragma once
#include "GameObject.h"

#define ID_ANI_FIREVENUSTRAP_LEFT 15100
#define ID_ANI_FIREVENUSTRAP_LEFT 15200


#define FIREVENUSTRAP_BBOX_WIDTH 16
#define FIREVENUSTRAP_BBOX_HEIGHT 32

#define FIREVENUSTRAP_SPEED 0.002f
#define FIREVENUSTRAP_FIREBALL_SPEED 0.005f
#define FIREVENUSTRAP_RANGE_MAX 128
#define FIREVENUSTRAP_RANGE_MIN 32


class CFireVenusTrap :public CGameObject
{
protected:
	float ay;

	virtual void GetBoundingBox(float l, float t, float r, float b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
public:

	CFireVenusTrap(float x, float y) :CGameObject(x, y) { vx = 0; vy = FIREVENUSTRAP_SPEED; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	
	virtual void SetState(int state);
};

