#pragma once
#include "GameObject.h"

#define BUTTON_STATE_NOT_CLICKED 1
#define BUTTON_STATE_CLICKED	 2

#define BUTTON_BBOX_WIDTH 16
#define BUTTON_BBOX_HEIGHT 16


#define ID_ANI_BUTTON_NOT_CLICKED 17100
#define ID_ANI_BUTTON_CLICKED	 17101

class CButton :public CGameObject
{
public:
	CButton(float x, float y) : CGameObject(x, y) { SetState(BUTTON_STATE_NOT_CLICKED); }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};

