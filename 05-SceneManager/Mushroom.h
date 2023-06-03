#pragma once
#include "GameObject.h"

#define ID_ANI_MUSHROOM 12100

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED_UP 0.001f
#define MUSHROOM_SPEED 0.04f

class CMushRoom :public CGameObject
{
protected:
	float xStart, yStart;
public:
	CMushRoom(float x, float y) :CGameObject(x, y) { xStart = x; yStart = y; }
	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsCollidable() { return 1; }

	void OnNoCollision(DWORD dt) {}

	void OnCollisionWith(LPCOLLISIONEVENT e);

	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

