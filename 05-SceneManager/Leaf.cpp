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

	if (GetState() == LEAF_STATE_UP)
	{
		if (y < yStart - LEAF_BBOX_HEIGHT*3)
		{
			SetState(LEAF_STATE_FLYING);
		}
	}
	if (abs(x-xStart)>= LEAF_BBOX_WIDTH * 2)
	{
		vx = -vx;
		xStart = x;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
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
		vx = LEAF_SPEED_FLYING;
		vy = LEAF_SPEED_DROP;
		break;
	}

}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}
