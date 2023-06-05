#pragma once
#include "GameObject.h"

#define LEAP_BBOX_HEIGHT 14
#define LEAP_BBOX_WIDTH 16

#define ID_ANI_LEAF_MOVING_RIGHT 14101
#define ID_ANI_LEAF_MOVING_LEFT 14102

class CLeaf :public CGameObject
{
protected:
	float xStart, yStart, ax, ay;
public:
	CLeaf(float x, float y) :CGameObject(x, y) { xStart = x; yStart = y; ax = 0; ay = 0; }
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsCollidable() { return 1; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

