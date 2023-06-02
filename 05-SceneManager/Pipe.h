#pragma once
#include "GameObject.h"

#define PIPE_BBOX_WIDTH 16
#define PIPE_BBOX_HEIGHT 16

class CPipe :public CGameObject
{
	int spriteId;
public:
	CPipe(float x, float y, int sprite_id) : CGameObject(x, y) { this->spriteId = sprite_id; }
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

