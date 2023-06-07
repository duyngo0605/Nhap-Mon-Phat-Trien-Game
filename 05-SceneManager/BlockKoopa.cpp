
#include "BlockKoopa.h"

void CBlockKoopa::Render()
{
	RenderBoundingBox();
}

void CBlockKoopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BLOCKKOOPA_BBOX_WIDTH / 2;
	t = y - BLOCKKOOPA_BBOX_HEIGHT / 2;
	r = l + BLOCKKOOPA_BBOX_WIDTH;
	b = t + BLOCKKOOPA_BBOX_HEIGHT;
}
