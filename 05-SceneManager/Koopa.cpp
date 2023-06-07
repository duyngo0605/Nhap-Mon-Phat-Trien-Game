#include "Koopa.h"
#include "Goomba.h"
#include "QuestionBrick.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Mario.h"
#include "PlayScene.h"

CKoopa::CKoopa(float x, float y, int type) :CGameObject(x, y)
{
	ax = 0;
	ay = KOOPA_GRAVITY;
	this->type = type;
	vx = -1;
	SetState(GOOMBA_STATE_WALKING);
}



void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_WALKING)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else
	{

		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_DEFEND / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_WIDTH;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isInCam())	return;
	vx += ax * dt;
	vy += vy * dt;
}

void CKoopa::OnNoCollision(DWORD dt)
{
	if (!isInCam())	return;
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!isInCam()) return;
	if (!e->obj->IsBlocking()) return;
	


	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	if (GetState() == KOOPA_STATE_THROWN)
	{
		if (dynamic_cast<CGoomba*>(e->obj)) {
			OnCollisionWithGoomba(e);
		}
		if (dynamic_cast<CQuestionBrick*>(e->obj)) {
			OnCollisionWithQuestionBrick(e);
		}
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	e->obj->SetState(GOOMBA_STATE_DIE);
	e->obj->Delete();
}

void CKoopa::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	if (!questionBrick->GetIsEmpty())
	{
		questionBrick->SetState(QUESTION_BRICK_STATE_UP);
		if (questionBrick->GetType() == QUESTION_BRICK_TYPE_COIN)
		{
			mario->AddCoin();
			CCoin* coin = new CCoin(x, y - QUESTION_BRICK_BBOX_HEIGHT);
			coin->SetSpeed(0, -0.5f);
			scene->AddObject(coin);
		}
		else if (questionBrick->GetType() == QUESTION_BRICK_TYPE_ITEM)
		{
			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				CMushRoom* mushroom = new CMushRoom(x, y);
				mushroom->SetState(MUSHROOM_STATE_UP);
				scene->AddObject(mushroom);
			}
			else
			{
				CLeaf* leaf = new CLeaf(x, y);
				leaf->SetState(LEAF_STATE_UP);
				scene->AddObject(leaf);
			}
		}
	}
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_WALKING:
		if (vx>0)
			vx = KOOPA_WALKING_SPEED;
		else
			vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_DEFEND:
		vx = 0;
		break;
	case KOOPA_STATE_THROWN:
		if (vx > 0)
			vx = KOOPA_THROWN_SPEED;
		else
			vx = -KOOPA_THROWN_SPEED;
		break;
	}
}
