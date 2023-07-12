#include "Card.h"

void CCard::Render()
{
	int aniId;
	CAnimations* animations = CAnimations::GetInstance();
	

	if (state == CARD_STATE_REFRESHING)
	{
		if (type == CARD_TYPE_MUSHROOM)
			aniId = ID_ANI_CARD_MUSHROOM;
		else if (type == CARD_TYPE_STAR)
			aniId = ID_ANI_CARD_STAR;
		else
			aniId = ID_ANI_CARD_FLOWER;
	}
	else if (state == CARD_STATE_UP)
	{
		if (type == CARD_TYPE_MUSHROOM)
			aniId = ID_ANI_CARD_MUSHROOM;
		else if (type == CARD_TYPE_STAR)
			aniId = ID_ANI_CARD_STAR;
		else
			aniId = ID_ANI_CARD_FLOWER;
		aniId += 100;
	}
	else
	{
		if (type == CARD_TYPE_MUSHROOM)
			aniId = ID_ANI_CARD_MUSHROOM;
		else if (type == CARD_TYPE_STAR)
			aniId = ID_ANI_CARD_STAR;
		else
			aniId = ID_ANI_CARD_FLOWER;
		aniId += 200;
	}
	animations->Get(aniId)->Render(x, y);
}

void CCard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
	if (state == CARD_STATE_REFRESHING)
	{
		if (GetTickCount64() - refresh_start > CARD_REFRESH_TIME)
			SetState(CARD_STATE_REFRESHING);
	}
	if (state == CARD_STATE_UP)
	{
		if (GetTickCount64() - up_start > CARD_UP_TIME)
			Delete();
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CCard::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - CARD_BBOX_WIDTH / 2;
	t = y - CARD_BBOX_HEIGHT / 2;
	r = l + CARD_BBOX_WIDTH;
	b = t + CARD_BBOX_HEIGHT;
}

void CCard::SetState(int state)
{
	switch (state)
	{
	case CARD_STATE_REFRESHING:
		refresh_start = GetTickCount64();
		vx = vy = 0;
		if (type < CARD_TYPE_FLOWER) type++;
		else
			type = CARD_TYPE_MUSHROOM;
		break;
	case CARD_STATE_UP:
		up_start = GetTickCount64();
		vy = CARD_SPEED_UP;
		break;
	case CARD_STATE_HUD:
		vx = vy = 0;
		break;
	}
	CGameObject::SetState(state);
}
