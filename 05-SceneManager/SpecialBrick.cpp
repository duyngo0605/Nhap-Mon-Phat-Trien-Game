#include "SpecialBrick.h"

void CSpecialBrick::Render()
{
	if (!isEmpty)
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_SPECIAL_BRICK)->Render(x, y);
	}
	else
	{
		CSprites* s = CSprites::GetInstance();
		s->Get(ID_EMPTY_QUESTION_BRICK_SPRITE)->Draw(x, y);
	}
	RenderBoundingBox();
}
