#include "BackgroundTile.h"

void CBackgroundTile::Render()
{
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteId)->Draw(x, y);
}
