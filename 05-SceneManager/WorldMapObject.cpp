#include "WorldMapObject.h"

void CWorldMapObject::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (vx < 0)
		animations->Get(aniId)->Render(x, y);
	else
		animations->Get(aniId+10)->Render(x, y);
}

void CWorldMapObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
	if (GetTickCount64() - changeDirect_start > CWORLDMAP_OBJECT_CHANGE_DIRECTION_TIME)
	{
		vx = -vx;
		changeDirect_start = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
}
