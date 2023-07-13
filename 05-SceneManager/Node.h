#pragma once
#include "Portal.h"

#define NODE_BBOX_WIDTH 0.001f
#define NODE_BBOX_HEIGHT 0.001f

class CNode :public CPortal
{
	int sprite_id;
	bool canPlay = false;

public:
	bool canMoveLeft;
	bool canMoveUp;
	bool canMoveRight;
	bool canMoveDown;
	CNode(float x, float y, int sprite_id, int l, int u, int r, int d):CPortal(x,y)
	{
		this->sprite_id = sprite_id;
		canMoveLeft = l;
		canMoveUp = u;
		canMoveRight = r;
		canMoveDown = d;
	}
	CNode(float x, float y, int sprite_id, int l, int u, int r, int d, int scene_id) :
		CPortal(x-NODE_BBOX_WIDTH/2, y-NODE_BBOX_HEIGHT/2,x+NODE_BBOX_WIDTH/2,y+NODE_BBOX_HEIGHT/2, scene_id){
		this->sprite_id = sprite_id;
		canPlay = true;
		canMoveLeft = l;
		canMoveUp = u;
		canMoveRight = r;
		canMoveDown = d;
	}
	void Render();

	bool IsCanPlay() { return canPlay; }
};

