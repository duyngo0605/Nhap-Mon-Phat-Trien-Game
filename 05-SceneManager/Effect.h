#pragma once
#include "GameObject.h"
#include "BackgroundTile.h"

#define EFFECT_SCORE_100	100
#define EFFECT_SCORE_200	200
#define EFFECT_SCORE_1000	1000
#define EFFECT_SCORE_1UP	1001

#define EFFECT_SPEED_Y	-0.1f
#define EFFECT_TIME_OUT	500

#define EFFECT_ADJUST_X		8.0f
#define EFFECT_ADJUST_Y		32

class CEffect :public CBackgroundTile
{
	ULONGLONG spawn_start;
public:
	CEffect(float x, float y, int sprite_id);
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

