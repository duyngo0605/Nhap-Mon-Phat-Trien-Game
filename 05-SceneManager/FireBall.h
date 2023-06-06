#pragma once
#include "GameObject.h"

#define FIREBALL_SPEED_X 0.03f
#define FIREBALL_SPEED_Y 0.02f

#define ID_ANI_FIREBALL 16100

#define FIREBALL_BBOX_WIDTH 9
#define FIREBALL_BBOX_HEIGHT 9

class CFireBall :public CGameObject
{
public:
	CFireBall(float x, float y) :CGameObject(x, y) {
		 vx = 0; vy = 0;
	}
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsCollidable() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

