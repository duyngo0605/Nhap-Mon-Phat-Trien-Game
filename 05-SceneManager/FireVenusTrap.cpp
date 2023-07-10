#include "FireVenusTrap.h"
#include "PlayScene.h"
#include "Mario.h"
#include "FireBall.h"

int CFireVenusTrap::GetAniIdRed()
{
	int aniId = -1;
	if (nx < 0) {
		if (ny > 0)
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				aniId = ID_ANI_RED_FIREVENUSTRAP_LEFT_DOWN + 5;
			else
				aniId = ID_ANI_RED_FIREVENUSTRAP_LEFT_DOWN;
		else
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				aniId = ID_ANI_RED_FIREVENUSTRAP_LEFT_UP + 5;
			else
				aniId = ID_ANI_RED_FIREVENUSTRAP_LEFT_UP;
	}
	else {
		if (ny > 0)
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				aniId = ID_ANI_RED_FIREVENUSTRAP_RIGHT_DOWN + 5;
			else
				aniId = ID_ANI_RED_FIREVENUSTRAP_RIGHT_DOWN;
		else
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				aniId = ID_ANI_RED_FIREVENUSTRAP_RIGHT_UP + 5;
			else
				aniId = ID_ANI_RED_FIREVENUSTRAP_RIGHT_UP;
	}
	return aniId;
}

int CFireVenusTrap::GetAniIdGreen()
{
	int aniId = -1;
	if (nx < 0) {
		if (ny > 0)
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				aniId = ID_ANI_GREEN_FIREVENUSTRAP_LEFT_DOWN + 5;
			else
				aniId = ID_ANI_GREEN_FIREVENUSTRAP_LEFT_DOWN;
		else
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				aniId = ID_ANI_GREEN_FIREVENUSTRAP_LEFT_UP + 5;
			else
				aniId = ID_ANI_GREEN_FIREVENUSTRAP_LEFT_UP;
	}
	else {
		if (ny > 0)
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				aniId = ID_ANI_GREEN_FIREVENUSTRAP_RIGHT_DOWN + 5;
			else
				aniId = ID_ANI_GREEN_FIREVENUSTRAP_RIGHT_DOWN;
		else
			if (GetState() == FIREVENUSTRAP_STATE_ATTACK)
				aniId = ID_ANI_GREEN_FIREVENUSTRAP_RIGHT_UP + 5;
			else
				aniId = ID_ANI_GREEN_FIREVENUSTRAP_RIGHT_UP;
	}
	return aniId;

}

void CFireVenusTrap::GetBoundingBox(float& l, float& t, float&  r, float& b)
{
	if (type == FIREVENUSTRAP_TYPE_RED)
	{
		l = x - FIREVENUSTRAP_BBOX_WIDTH / 2;
		t = y - FIREVENUSTRAP_RED_BBOX_HEIGHT / 2;
		r = l + FIREVENUSTRAP_BBOX_WIDTH;
		b = t + FIREVENUSTRAP_RED_BBOX_HEIGHT;
	}
	else
	{
		l = x - FIREVENUSTRAP_BBOX_WIDTH / 2;
		t = y - FIREVENUSTRAP_GREEN_BBOX_HEIGHT / 2;
		r = l + FIREVENUSTRAP_BBOX_WIDTH;
		b = t + FIREVENUSTRAP_GREEN_BBOX_HEIGHT;

	}

}

void CFireVenusTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)(scene->GetPlayer());
	float xM, yM;
	mario->GetPosition(xM, yM);
	if (xM - x < 0) nx = -1;
	else nx = 1;
	if (yM - y < 0) ny = -1;
	else ny = 1;
	if (GetState() == FIREVENUSTRAP_STATE_DIE && (GetTickCount64() - die_start > FIREVENUSTRAP_DIE_TIME_OUT))
	{
		Delete();
		return;
	}
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
		if(type==FIREVENUSTRAP_TYPE_RED)
		{ 
			if (y <= yStart - FIREVENUSTRAP_RED_BBOX_HEIGHT)
				SetState(FIREVENUSTRAP_STATE_ATTACK);
		}
		else
		{
			if (y <= yStart - FIREVENUSTRAP_GREEN_BBOX_HEIGHT)
				SetState(FIREVENUSTRAP_STATE_ATTACK);
		}
		
	}
	else if (GetState() == FIREVENUSTRAP_STATE_ATTACK) {
		
		if (GetTickCount64() - attack_start == FIREVENUSTRAP_TIME_ATTACK / 2 && type!=VENUSTRAP_TYPE)
		{
			CFireBall* fire = new CFireBall(x, y-FIREBALL_BBOX_HEIGHT);
			fire->SetSpeed(nx * FIREBALL_SPEED_X, ny * FIREBALL_SPEED_Y);
			scene->AddObject(fire);
		}
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
	int aniId = -1;
	if (GetState() == FIREVENUSTRAP_STATE_DIE)
	{
		aniId = ID_ANI_MARIO_FROM_BIG_TO_TAIL_RIGHT;
	}
	else {
		if (type == FIREVENUSTRAP_TYPE_RED)
		{
			aniId = GetAniIdRed();
		}
		else if (type == FIREVENUSTRAP_TYPE_GREEN)
			aniId = GetAniIdGreen();
		else
			aniId = ID_ANI_VENUSTRAP;
	}
	if (aniId == -1)return;
	animations->Get(aniId)->Render(x, y);
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
	case FIREVENUSTRAP_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		vy = 0;
		
		break;
	}
}