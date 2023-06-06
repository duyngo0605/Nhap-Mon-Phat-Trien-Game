#pragma once
#include "GameObject.h"

#define ID_ANI_FIREVENUSTRAP_LEFT_DOWN 15101
#define ID_ANI_FIREVENUSTRAP_LEFT_UP 15102

#define ID_ANI_FIREVENUSTRAP_RIGHT_DOWN 15111
#define ID_ANI_FIREVENUSTRAP_RIGHT_UP 15112


#define FIREVENUSTRAP_BBOX_WIDTH 16
#define FIREVENUSTRAP_BBOX_HEIGHT 32

#define FIREVENUSTRAP_SPEED 0.002f
#define FIREVENUSTRAP_FIREBALL_SPEED 0.005f
#define FIREVENUSTRAP_RANGE_MAX 128
#define FIREVENUSTRAP_RANGE_MIN 32

#define FIREVENUSTRAP_STATE_HIDE 100
#define FIREVENUSTRAP_STATE_UP 200
#define FIREVENUSTRAP_STATE_DOWN 300
#define FIREVENUSTRAP_STATE_ATTACK 400


class CFireVenusTrap :public CGameObject
{
protected:
	float xStart, yStart, ny;
	
public:
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CFireVenusTrap(float x, float y) :CGameObject(x, y) { vx = 0; vy = 0; xStart = x; yStart = y; state = FIREVENUSTRAP_STATE_HIDE; ny = 0; }
	void OnNoCollision(DWORD dt);
	void SetState(int state);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
};

