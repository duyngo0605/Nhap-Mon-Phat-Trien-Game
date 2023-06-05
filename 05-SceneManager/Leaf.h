#pragma once
#include "GameObject.h"

#define LEAF_BBOX_HEIGHT 14
#define LEAF_BBOX_WIDTH 16

#define ID_ANI_LEAF_MOVING_RIGHT 14101
#define ID_ANI_LEAF_MOVING_LEFT 14102

#define LEAF_STATE_UP 100
#define LEAF_STATE_FLYING 200

#define LEAF_SPEED_UP -0.25f
#define LEAF_SPEED_FLYING 0.05f

#define LEAF_SPEED_DROP 0.025f

class CLeaf :public CGameObject
{
protected:
	float xStart, yStart;
public:
	CLeaf(float x, float y) :CGameObject(x, y) { xStart = x; yStart = y; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsCollidable() { return 0; }

	void OnNoCollision(DWORD dt);

	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

