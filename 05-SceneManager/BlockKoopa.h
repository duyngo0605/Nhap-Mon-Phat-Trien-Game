#pragma once
#include "GameObject.h"

#define BLOCKKOOPA_BBOX_WIDTH 16
#define BLOCKKOOPA_BBOX_HEIGHT 16

class CBlockKoopa :
    public CGameObject
{
public:
	CBlockKoopa(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	int IsCollidable() { return 0; }
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

