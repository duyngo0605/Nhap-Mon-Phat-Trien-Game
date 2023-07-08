#pragma once
#include "QuestionBrick.h"

#define ID_ANI_SPECIAL_BRICK 15001

#define SPECIAL_BRICK_TYPE_NORMAL 0
#define SPECIAL_BRICK_TYPE_MUSHROOM 1
#define SPECIAL_BRICK_TYPE_BUTTON 2

#define NUM_OF_PIECES 4

class CSpecialBrick : public CQuestionBrick
{
public:
	CSpecialBrick(float x, float y, int type) :CQuestionBrick(x, y, type){}
	void Render();
	void Break();
};

