#pragma once
#include "BackgroundTile.h"
class CLandScape :public CBackgroundTile
{
public:
	CLandScape(float x, float y, int sprite_id) :CBackgroundTile(x, y, sprite_id){}
	void Render();
	int IsBlocking() { return 1; }
};

