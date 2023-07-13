#pragma once
#include "GameObject.h"

#define ID_ANI_ARROW	45100

#define ARROW_STATE_UP		100
#define	ARROW_STATE_DOWN	200

#define ARROW_X				72
#define ARROW_UP_Y			148
#define ARROW_DOWN_Y		164

class CArrow :public CGameObject
{
public:
	CArrow(float x, float y): CGameObject(x,y){
		CGameObject::SetState(ARROW_STATE_UP);
	}
	void Render();
	void SetState(int state);
	void EnterWorldMap();
	void GetBoundingBox(float& l, float& t, float& r, float& b){}
};

