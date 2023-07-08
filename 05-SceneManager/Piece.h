#pragma once
#include "GameObject.h"

#define ID_ANI_PIECE 15999

#define PIECE_SPEED_X 0.2f
#define PIECE_GRAVITY 0.001f

#define PIECE_TIMEOUT 1000

#define PIECE_BBOX_WIDTH 8
#define PIECE_BBOX_HEIGHT 8

/// pieces of special brick broken

class CPiece :  public CGameObject
{
	ULONGLONG spawn_start;
public:
	CPiece(float x, float y) : CGameObject(x, y) { spawn_start = GetTickCount64(); }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

