#include "Effect.h"
#include "PlayScene.h"

CEffect::CEffect(float x, float y, int sprite_id):CBackgroundTile(x-EFFECT_ADJUST_X,y- EFFECT_ADJUST_Y,sprite_id)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vx = 0;
	vy = EFFECT_SPEED_Y;
	spawn_start = GetTickCount64();
	scene->AddObject(this);
}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>*coObjects)
{
	x += vx * dt;
	y += vy * dt;
	if (GetTickCount64() - spawn_start > EFFECT_TIME_OUT)
		Delete();
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
