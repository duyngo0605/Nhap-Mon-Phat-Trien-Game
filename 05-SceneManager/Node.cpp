#include "Node.h"

void CNode::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(sprite_id)->Draw(x, y);
}
