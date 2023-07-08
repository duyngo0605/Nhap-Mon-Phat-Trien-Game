#pragma once
#include "GameObject.h"

#define ID_ANI_PIECE 15999

#define PIECE_SPEED_X 0.4f
#define PIECE_GRAVITY 0.002f

#define PIECE_TIMEOUT 200

/// pieces of special brick broken

class CPiece :  public CGameObject
{
	ULONGLONG spawn_start;
public:
	CPiece(float x, float y, int ny) : CGameObject(x, y) { spawn_start = GetTickCount64(); vx = PIECE_SPEED_X; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	int IsBlocking() { return 0; }
};

