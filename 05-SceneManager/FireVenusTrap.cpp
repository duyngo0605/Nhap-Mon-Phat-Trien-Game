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
	if (GetState() == FIREVENUSTRAP_STATE_HIDE)
	{
		if (GetTickCount64() - attack_gap >= FIREVENUSTRAP_TIME_ATTACK)
		{
			if (abs(xM - x) <= FIREVENUSTRAP_RANGE_MAX && abs(xM - x) >= FIREVENUSTRAP_RANGE_MIN)
			{
				SetState(FIREVENUSTRAP_STATE_UP);
			}
		}
	}
	else if (GetState() == FIREVENUSTRAP_STATE_UP)
	{
		if (y <= yStart - FIREVENUSTRAP_BBOX_HEIGHT*1.25)
			SetState(FIREVENUSTRAP_STATE_ATTACK);
	}
	else if (GetState() == FIREVENUSTRAP_STATE_ATTACK) {
		if (GetTickCount64() - attack_start>=FIREVENUSTRAP_TIME_ATTACK/2)
		if (GetTickCount64() - attack_start >= FIREVENUSTRAP_TIME_ATTACK)
			SetState(FIREVENUSTRAP_STATE_DOWN);
	}
	else if (GetState() == FIREVENUSTRAP_STATE_DOWN)
	{
		if (y >= yStart)
			SetState(FIREVENUSTRAP_STATE_HIDE);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFireVenusTrap::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (nx < 0) {
		if(ny < 0)
			if (GetState()==FIREVENUSTRAP_STATE_ATTACK)
				animations->Get(ID_ANI_FIREVENUSTRAP_LEFT_DOWN+5)->Render(x, y);
			else
				animations->Get(ID_ANI_FIREVENUSTRAP_LEFT_DOWN)->Render(x, y);
		else
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				animations->Get(ID_ANI_FIREVENUSTRAP_LEFT_UP + 5)->Render(x, y);
			else
			animations->Get(ID_ANI_FIREVENUSTRAP_LEFT_UP)->Render(x, y);
	}
	else {
		if (ny < 0)
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				animations->Get(ID_ANI_FIREVENUSTRAP_RIGHT_DOWN + 5)->Render(x, y);
			else
				animations->Get(ID_ANI_FIREVENUSTRAP_RIGHT_DOWN)->Render(x, y);
		else
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				animations->Get(ID_ANI_FIREVENUSTRAP_RIGHT_UP + 5)->Render(x, y);
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
	CGameObject::SetState(state);
	switch (this->state)
	{
	case FIREVENUSTRAP_STATE_HIDE:
		attack_gap = GetTickCount64();
		y = yStart;
		vy = 0;
		break;
	case FIREVENUSTRAP_STATE_UP:
		SetSpeed(0,-FIREVENUSTRAP_SPEED);
		break;
	case FIREVENUSTRAP_STATE_ATTACK:
		attack_start = GetTickCount64();
		vy = 0;
		break;
	case FIREVENUSTRAP_STATE_DOWN:
		SetSpeed(0, FIREVENUSTRAP_SPEED);
		break;
	}
}
