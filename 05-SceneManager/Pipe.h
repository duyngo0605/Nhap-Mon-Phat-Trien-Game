#pragma once
#include "GameObject.h"

#define PIPE_BBOX_WIDTH 16
#define PIPE_BBOX_HEIGHT 16

class CPipe :public CGameObject
{
protected:
	int spriteId;
public:
	CPipe(float x, float y, int sprite_id) : CGameObject(x, y) { this->spriteId = sprite_id; }
	void Render();
	void Update(DWORD dt) {}
	int IsCollidable() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

