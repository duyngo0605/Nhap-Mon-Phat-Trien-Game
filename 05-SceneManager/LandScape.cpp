#include "LandScape.h"

void CLandScape::Render()
{
	CAnimations* animation = CAnimations::GetInstance();
	animation->Get(spriteId)->Render(x, y);
}
