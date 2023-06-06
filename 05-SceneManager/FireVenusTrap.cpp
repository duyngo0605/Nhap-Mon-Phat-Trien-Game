#include "FireVenusTrap.h"

void CFireVenusTrap::GetBoundingBox(float l, float t, float r, float b)
{
	l = x - FIREVENUSTRAP_BBOX_WIDTH / 2;
	t = y - FIREVENUSTRAP_BBOX_HEIGHT / 2;
	r = l + FIREVENUSTRAP_BBOX_WIDTH;
	b = t + FIREVENUSTRAP_BBOX_HEIGHT;

}
