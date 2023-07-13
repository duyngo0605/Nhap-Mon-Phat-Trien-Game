#pragma once
#include "Portal.h"

#define NODE_BBOX_WIDTH 16
#define NODE_BBOX_HEIGHT 16

class CNode :public CPortal
{
	int sprite_id;
	bool canPlay = false;
public:
	CNode(float x, float y, int sprite_id):CPortal(x,y)
	{
		this->sprite_id = sprite_id;
	}
	CNode(float x, float y, int sprite_id, int scene_id) :
		CPortal(x-NODE_BBOX_WIDTH/2, y-NODE_BBOX_HEIGHT/2,x+NODE_BBOX_WIDTH/2,y+NODE_BBOX_HEIGHT/2, scene_id){
		this->sprite_id = sprite_id;
		canPlay = true;
	}
	void Render();

	bool IsCanPlay() { return canPlay; }
};

