#include "Leaf.h"

void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (vx>=0)
		animations->Get(ID_ANI_LEAF_MOVING_RIGHT)->Render(x, y);
	else
		animations->Get(ID_ANI_LEAF_MOVING_LEFT)->Render(x, y);
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CLeaf::OnNoCollision(DWORD dt)
{
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CLeaf::SetState(int state)
{
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}
