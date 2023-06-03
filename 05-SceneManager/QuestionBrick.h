#pragma once
#include "GameObject.h"

#define ID_ANI_QUESTION_BRICK 15000
#define ID_EMPTY_QUESTION_BRICK_SPRITE 21004

#define QUESTION_BRICK_BBOX_WIDTH 16
#define QUESTION_BRICK_BBOX_HEIGHT 16

#define COIN_QUESTION_BRICK 1
#define ITEM_QUESTION_BRICK 2

class CQuestionBrick : public CGameObject
{
	boolean isEmpty=false;
	int type;

public:
	CQuestionBrick(float x, float y, int type) : CGameObject(x, y) { this->type = type; }
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

