#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_GRAVITY 0.05f
#define COIN_MAX_VY 0.5f

#define COIN_STATE_NORMAL 1
#define COIN_STATE_OUT 2

#define COIN_SPEED_OUT -0.5f

class CCoin : public CGameObject {
protected:
	float yStart;
	float xStart;
public:
	CCoin(float x, float y) : CGameObject(x, y) { xStart = x; yStart = y; state = COIN_STATE_NORMAL; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	int IsBlocking() { return 0; }
};