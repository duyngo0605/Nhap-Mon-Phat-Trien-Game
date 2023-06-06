#pragma once
#include "GameObject.h"

#define FIREBALL_SPEED_X 0.03f
#define FIREBALL_SPEED_Y 0.02f

#define ID_ANI_FIREBALL 16100

#define FIREBALL_BBOX_WIDTH 9
#define FIREBALL_BBOX_HEIGHT 9

class CFireBall :public CGameObject
{
protected:
	int nx, ny;
public:
	CFireBall(float x, float y, int nx, int ny) :CGameObject(x, y) {
		this->nx = nx; this->ny = ny; vx = nx * FIREBALL_SPEED_X; vy = ny * FIREBALL_SPEED_Y;
	}
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsCollidable() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

