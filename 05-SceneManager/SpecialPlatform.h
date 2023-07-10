#pragma once
#include "Platform.h"
#include "BlockKoopa.h"
#include "PlayScene.h"

#define SPECIAL_PLATFORM_BBOX_HEIGHT 16
#define SPECIAL_PLATFORM_BBOX_WIDTH 16

class CSpecialPlatform :public CPlatform //only block with down moving
{
public:
	CSpecialPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CPlatform(x, y,
			cell_width, cell_height, length,
			sprite_id_begin, sprite_id_middle, sprite_id_end){
		CBlockKoopa* begin = new CBlockKoopa(x - BLOCKKOOPA_BBOX_WIDTH, y - BLOCKKOOPA_BBOX_HEIGHT);
		CBlockKoopa* end = new CBlockKoopa(x + cell_width*length + BLOCKKOOPA_BBOX_WIDTH/2, y - BLOCKKOOPA_BBOX_HEIGHT);
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(begin);
		scene->AddObject(end);
	}
	int IsBlocking() { return 0; }
};

