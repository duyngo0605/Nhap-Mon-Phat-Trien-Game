#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define FIRE_BALL_VY -0.2f
#define FIRE_BALL_VX 0.5f
#define ID_ANI_FIREBALL 20000

class CBrick : public CGameObject {
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
};

class CFireball : public CGameObject
{
	float vy;				// acceleration on x 
	float vx;
public:
	CFireball(float x, float y, float per) : CGameObject(x, y)
	{
		vy = 0;
		if (per == 0)
			vx = -FIRE_BALL_VX;
		else
			vx = FIRE_BALL_VX;
	}
	void Update(DWORD dt);
	void Render();
};