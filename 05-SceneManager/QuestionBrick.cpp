#include "QuestionBrick.h"

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

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BRICK_BBOX_WIDTH / 2;
	t = y - QUESTION_BRICK_BBOX_HEIGHT / 2;
	r = l + QUESTION_BRICK_BBOX_WIDTH;
	b = t + QUESTION_BRICK_BBOX_HEIGHT;
}