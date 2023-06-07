#include "QuestionBrick.h"

CQuestionBrick::CQuestionBrick(float x, float y, int type) :CGameObject(x, y)
{
	this->type = type; 
	xStart = x; 
	yStart = y; 	
	yMin = y - QUESTION_BRICK_BBOX_HEIGHT / 4;
	state = QUESTION_BRICK_STATE_DEFAULT;
}

void CQuestionBrick::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CQuestionBrick::Render()
{

	if (!isEmpty)
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_QUESTION_BRICK)->Render(x, y);
	}
	else
	{
		CSprites* s = CSprites::GetInstance();
		s->Get(ID_EMPTY_QUESTION_BRICK_SPRITE)->Draw(x, y);
	}
	RenderBoundingBox();
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	x += vx * dt;
	y += vy * dt;

	if (y <= yMin)
	{
		vy = QUESTION_BRICK_SPEED_UP;
		isEmpty = true;
	}
	if (y >= yStart)
	{
		SetState(QUESTION_BRICK_STATE_DEFAULT);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BRICK_BBOX_WIDTH / 2;
	t = y - QUESTION_BRICK_BBOX_HEIGHT / 2;
	r = l + QUESTION_BRICK_BBOX_WIDTH;
	b = t + QUESTION_BRICK_BBOX_HEIGHT;
}


void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BRICK_STATE_UP:
		vy = -QUESTION_BRICK_SPEED_UP;
		break;
	case QUESTION_BRICK_STATE_DEFAULT:
		x = xStart;
		y = yStart;
		vy = 0;
		break;

	}
}
	
