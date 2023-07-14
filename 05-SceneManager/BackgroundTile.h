#pragma once
#include "GameObject.h"


///use to draw background of map

class CBackgroundTile :
    public CGameObject
{
protected:
	int spriteId;
public:
	CBackgroundTile(float x, float y, int sprite_id) : CGameObject(x, y) { this->spriteId = sprite_id; }
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	int IsBlocking() { return 0; }
};

