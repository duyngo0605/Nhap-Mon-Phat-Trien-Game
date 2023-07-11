#pragma once
#include "GameObject.h"

#define PIPE_BBOX_WIDTH 16
#define PIPE_BBOX_HEIGHT 16

class CPipe :public CGameObject
{
protected:
	int spriteId;
	bool isCanGo = false;
	float xD, yD; ///destination location
public:
	CPipe(float x, float y, int sprite_id) : CGameObject(x, y) { this->spriteId = sprite_id; this->xD = xD;
	this->yD = yD; isCanGo = false;
	}
	CPipe(float x, float y, int sprite_id, int canDown, float xD, float yD) : CGameObject(x, y) 
	{ 
		this->spriteId = sprite_id; 
		isCanGo = (canDown == 1); 
		this->xD = xD;
		this->yD = yD;
	}
	float GetDestinationX() { return xD; }
	float GetDestinationY() { return yD; }
	bool GetIsCanGo() { return isCanGo; }
	void Render();
	void Update(DWORD dt) {}
	int IsCollidable() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

