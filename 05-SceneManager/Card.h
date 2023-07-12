#pragma once
#include "GameObject.h"

#define CARD_REFRESH_TIME	50
#define CARD_UP_TIME		200

#define ID_ANI_CARD_MUSHROOM 19100
#define ID_ANI_CARD_STAR	 19101
#define ID_ANI_CARD_FLOWER	 19102

#define CARD_TYPE_MUSHROOM	1
#define CARD_TYPE_STAR		2
#define CARD_TYPE_FLOWER	3

#define CARD_STATE_REFRESHING 100
#define CARD_STATE_UP		 200
#define CARD_STATE_HUD		 300

#define CARD_BBOX_WIDTH		16
#define CARD_BBOX_HEIGHT	16

#define CARD_SPEED_UP		-0.25f

class CCard :public CGameObject
{
	int type;
	ULONGLONG refresh_start, up_start;
public:
	CCard(float x, float y) : CGameObject(x, y) {
		state = CARD_STATE_REFRESHING;
		type = CARD_TYPE_MUSHROOM;
		refresh_start = up_start = -1;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	int IsBlocking() { return 0; }
};

