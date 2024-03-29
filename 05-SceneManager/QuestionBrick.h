#pragma once
#include "GameObject.h"

#define ID_ANI_QUESTION_BRICK 15000
#define ID_EMPTY_QUESTION_BRICK_SPRITE 21004

#define QUESTION_BRICK_BBOX_WIDTH 16
#define QUESTION_BRICK_BBOX_HEIGHT 16

#define QUESTION_BRICK_TYPE_COIN 1
#define QUESTION_BRICK_TYPE_ITEM 2

#define QUESTION_BRICK_STATE_UP 100
#define QUESTION_BRICK_STATE_DEFAULT 200
#define QUESTION_BRICK_SPEED_UP 0.05f

class CQuestionBrick : public CGameObject
{
protected:

	float xStart;
	float yStart;
	float yMin;
	boolean isEmpty=false;
	int type;

public:
	CQuestionBrick(float x, float y, int type);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnNoCollision(DWORD dt);
	boolean GetIsEmpty() { return isEmpty; }
	int GetType() { return this->type; }
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

