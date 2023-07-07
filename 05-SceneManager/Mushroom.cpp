#include "Mushroom.h"
#include "Mario.h"
#include "PlayScene.h"
#include "SpecialPlatform.h"

void CMushRoom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (type==MUSHROOM_TYPE_RED)
		animations->Get(ID_ANI_RED_MUSHROOM)->Render(x, y);
	else
		animations->Get(ID_ANI_GREEN_MUSHROOM)->Render(x, y);
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += ax * dt;
	vy += ay * dt;
	if (GetState() == MUSHROOM_STATE_UP)
	{
		if (y <= yStart - MUSHROOM_BBOX_HEIGHT)
		{
			SetState(MUSHROOM_STATE_MOVING);
		}
	}
	if (abs(vx) >= MUSHROOM_MAX_SPEED)
	{
		ax = 0;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushRoom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushRoom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CSpecialPlatform*>(e->obj))
		OnCollisionWithSpecialPlatform(e);
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0&&e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushRoom::OnCollisionWithSpecialPlatform(LPCOLLISIONEVENT e)
{
	CSpecialPlatform* specialPlat = dynamic_cast<CSpecialPlatform*>(e->obj);
	float xPlat, yPlat;
	specialPlat->GetPosition(xPlat, yPlat);
	if (e->ny < 0) {
		if (yPlat - y <= (MUSHROOM_BBOX_HEIGHT))
		{
			SetPosition(x, yPlat - MUSHROOM_BBOX_HEIGHT);
			vy = 0;
		}
	}
}

void CMushRoom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushRoom::SetState(int state)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float xM, yM;
	mario->GetPosition(xM, yM);
	CGameObject::SetState(state);
	
	switch (this->state)
	{

	case MUSHROOM_STATE_UP:
		SetSpeed(0, MUSHROOM_SPEED_UP);
		break;

	case MUSHROOM_STATE_MOVING:
		ax = MUSHROOM_AX;
		ay = MUSHROOM_GRAVITY;
		if (x < xM) vx = -MUSHROOM_SPEED;
		else vx = MUSHROOM_SPEED;
		break;
	}

}
