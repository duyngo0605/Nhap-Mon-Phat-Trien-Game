#include "Piece.h"

void CPiece::Render()
{
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_PIECE)->Render(x, y);

	}
}

void CPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
	vy += PIECE_GRAVITY * dt;
	if (GetTickCount64() - spawn_start > PIECE_TIMEOUT)
		Delete();
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
