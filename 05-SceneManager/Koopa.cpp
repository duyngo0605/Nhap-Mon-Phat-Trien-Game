#include "Koopa.h"
#include "Goomba.h"
#include "QuestionBrick.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Mario.h"
#include "PlayScene.h"
#include "SpecialPlatform.h"
#include "BlockKoopa.h"

CKoopa::CKoopa(float x, float y, int type) :CGameObject(x, y)
{
	ax = 0;
	ay = KOOPA_GRAVITY;
	this->type = type;
	vx = 0;
	SetState(GOOMBA_STATE_WALKING);
}


int CKoopa::GetAniIdRed()
{
	int aniId = -1;
	if (vx == KOOPA_WALKING_SPEED)
		aniId = ID_ANI_RED_KOOPA_WALK_RIGHT;
	else if (vx == -KOOPA_WALKING_SPEED)
		aniId = ID_ANI_RED_KOOPA_WALK_LEFT;
	else if (abs(vx) == KOOPA_THROWN_SPEED)
		aniId = ID_ANI_RED_KOOPA_THROWN;
	else if (vx == 0)
		aniId = ID_ANI_RED_KOOPA_DEFEND;
	return aniId;
}

int CKoopa::GetAniIdGreen()
{
	return 0;
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
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	int aniId = -1;
	if (type == KOOPA_TYPE_RED)
		aniId = GetAniIdRed();
	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
		
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
	if (GetState() == KOOPA_STATE_THROWN)
	{
		if (dynamic_cast<CGoomba*>(e->obj)) {
			OnCollisionWithGoomba(e);
		}
		if (dynamic_cast<CQuestionBrick*>(e->obj)) {
			OnCollisionWithQuestionBrick(e);
		}
	}
	else
	{
		if (dynamic_cast<CBlockKoopa*>(e->obj)) {
			OnCollisionWithBlockKoopa(e);
		}
	}
	if (dynamic_cast<CSpecialPlatform*>(e->obj))
		OnCollisionWithSpecialPlatform(e);
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	
}

void CKoopa::OnCollisionWithSpecialPlatform(LPCOLLISIONEVENT e)
{
	CSpecialPlatform* specialPlat = dynamic_cast<CSpecialPlatform*>(e->obj);
	float xPlat, yPlat;
	specialPlat->GetPosition(xPlat, yPlat);
	if (e->ny < 0) {
		if (state == KOOPA_STATE_WALKING)
		{
			if (yPlat - y <= (KOOPA_BBOX_HEIGHT+SPECIAL_PLATFORM_BBOX_HEIGHT)/2)
			{
				SetPosition(x, yPlat - (KOOPA_BBOX_HEIGHT + SPECIAL_PLATFORM_BBOX_HEIGHT) / 2);
				vy = 0;
			}
		}
		else
		{
			if (yPlat - y <= (KOOPA_BBOX_HEIGHT_DEFEND))
			{
				SetPosition(x, yPlat - KOOPA_BBOX_HEIGHT_DEFEND);
				vy = 0;
			}
		}
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	goomba->SetState(GOOMBA_STATE_DIE);
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

void CKoopa::OnCollisionWithBlockKoopa(LPCOLLISIONEVENT e)
{
	if (e->nx != 0)
	{
		vx = -vx;
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
