#pragma once
#include "Platform.h"

#define SPECIAL_PLATFORM_BBOX_HEIGHT 16
#define SPECIAL_PLATFORM_BBOX_WIDTH 16

class CSpecialPlatform :public CPlatform //only block with down moving
{
public:
	CSpecialPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CPlatform(x, y,
			cell_width, cell_height, length,
			sprite_id_begin, sprite_id_middle, sprite_id_end){}
	int IsBlocking() { return 0; }
};

