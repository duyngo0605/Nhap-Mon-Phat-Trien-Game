#pragma once
#include "GameObject.h"

#define CWORLDMAP_OBJECT_CHANGE_DIRECTION_TIME	2000

class CWorldMapObject :public CGameObject
{
	int aniId;
	ULONGLONG changeDirect_start;
public:
	CWorldMapObject(float x, float y, float vx, float vy, int aniId) :CGameObject(x, y)
	{
		this->vx = vx;
		this->vy = vy;
		this->aniId = aniId;
		changeDirect_start = GetTickCount64();
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
};

