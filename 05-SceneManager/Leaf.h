#pragma once
#include "GameObject.h"

#define LEAF_BBOX_HEIGHT 14
#define LEAF_BBOX_WIDTH 16

#define ID_ANI_LEAF_MOVING_RIGHT 14101
#define ID_ANI_LEAF_MOVING_LEFT 14102

#define LEAF_STATE_UP 100
#define LEAF_STATE_FLYING 200

#define LEAF_SPEED _UP 0.25f
#define LEAF_SPEED_FLYING 0.15f

#define LEAF_GRAVITY 0.001f

class CLeaf :public CGameObject
{
protected:
	float xStart, yStart, ax, ay;
public:
	CLeaf(float x, float y) :CGameObject(x, y) { xStart = x; yStart = y; ax = 0; ay = LEAF_GRAVITY; }
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsCollidable() { return 1; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

