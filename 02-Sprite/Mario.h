#pragma once

#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#define MARIO_WIDTH 14

class CBrick : public CGameObject {
	int type;
public: 
	CBrick(float x, float y) : CGameObject(x, y) {}
	CBrick(float x, float y, int type) : CGameObject(x, y) { this->type = type; }
	void Render();
	void Update(DWORD dt) {}
};

class CMario : public CGameObject
{
protected:
	float vx;
public:
	CMario(float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};


