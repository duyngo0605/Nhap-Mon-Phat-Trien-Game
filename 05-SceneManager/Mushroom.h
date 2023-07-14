#pragma once
#include "GameObject.h"

#define ID_ANI_RED_MUSHROOM 13100
#define ID_ANI_GREEN_MUSHROOM 13101

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED_UP -0.02f
#define MUSHROOM_SPEED 0.03f
#define MUSHROOM_MAX_SPEED 0.05f

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_AX 0.001f

#define MUSHROOM_STATE_MOVING 1
#define MUSHROOM_STATE_UP 2

#define MUSHROOM_TYPE_RED 1
#define MUSHROOM_TYPE_GREEN 2

class CMushRoom :public CGameObject
{
protected:
	float xStart, yStart, ax, ay;
	int type;
public:
	CMushRoom(float x, float y, int type) :CGameObject(x, y) { xStart = x; yStart = y; ax = 0; ay = 0; this->type = type; }
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsCollidable() { return 1; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithSpecialPlatform(LPCOLLISIONEVENT e);

	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int GetType() { return type; }
};

