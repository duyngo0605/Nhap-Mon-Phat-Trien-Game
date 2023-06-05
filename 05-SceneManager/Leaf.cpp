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

	vx += ax * dt;
	vy += ay * dt;
	if (GetState() == LEAF_STATE_UP)
	{
		if (y < yStart - LEAF_BBOX_HEIGHT*3)
		{
			SetState(LEAF_STATE_FLYING);
		}
	}
	if (abs(vx) >= LEAF_SPEED_FLYING)
	{
		ax = -ax;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::OnNoCollision(DWORD dt)
{
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);

	switch (this->state)
	{

	case LEAF_STATE_UP:
		SetSpeed(0, LEAF_SPEED_UP);
		break;

	case LEAF_STATE_FLYING:
		ax = LEAF_AX;
		ay = LEAF_GRAVITY;
		vy = 0;
		break;
	}

}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}
