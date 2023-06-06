#include "FireVenusTrap.h"
#include "PlayScene.h"
#include "Mario.h"

void CFireVenusTrap::GetBoundingBox(float& l, float& t, float&  r, float& b)
{
	l = x - FIREVENUSTRAP_BBOX_WIDTH / 2;
	t = y - FIREVENUSTRAP_BBOX_HEIGHT / 2;
	r = l + FIREVENUSTRAP_BBOX_WIDTH;
	b = t + FIREVENUSTRAP_BBOX_HEIGHT;

}

void CFireVenusTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float xM, yM;
	mario->GetPosition(xM, yM);
	if (xM - x < 0) nx = -1;
	else nx = 1;
	if (yM - y < 0) ny = 1;
	else ny = -1;
	if (abs(xM-x) > FIREVENUSTRAP_RANGE_MAX||abs(xM-x)<FIREVENUSTRAP_RANGE_MIN) return;
		SetState(FIREVENUSTRAP_STATE_ATTACK);
}

void CFireVenusTrap::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (nx < 0) {
		if(ny < 0)
			animations->Get(ID_ANI_FIREVENUSTRAP_LEFT_DOWN)->Render(x, y);
		else
			animations->Get(ID_ANI_FIREVENUSTRAP_LEFT_UP)->Render(x, y);
	}
	else {
		if (ny < 0)
			animations->Get(ID_ANI_FIREVENUSTRAP_RIGHT_DOWN)->Render(x, y);
		else
			animations->Get(ID_ANI_FIREVENUSTRAP_RIGHT_UP)->Render(x, y);
	}
}

void CFireVenusTrap::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireVenusTrap::SetState(int state)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float xM, yM;
	mario->GetPosition(xM, yM);
	CGameObject::SetState(state);
	switch (state)
	{
	case FIREVENUSTRAP_STATE_HIDE:
		vy = 0;
		break;
	case FIREVENUSTRAP_STATE_UP:
		vy = FIREVENUSTRAP_SPEED;
	}
}
