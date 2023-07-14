#pragma once
#include "GameObject.h"
#include "PlayScene.h"

#define ID_ANI_ARROW	50100

#define ARROW_STATE_UP		100
#define	ARROW_STATE_DOWN	200

#define ARROW_DISTANCE_Y	16.0f

class CArrow :public CGameObject
{
	float distance;
public:
	CArrow(float x, float y): CGameObject(x,y){
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		if (scene->GetId() == ID_SCENE_INTRO)
			distance = ARROW_DISTANCE_Y;
		else
			distance = ARROW_DISTANCE_Y / 2;
		CGameObject::SetState(ARROW_STATE_UP);
	}
	void Render();
	void SetState(int state);
	void SelectOpion();
	void GetBoundingBox(float& l, float& t, float& r, float& b){}
};

