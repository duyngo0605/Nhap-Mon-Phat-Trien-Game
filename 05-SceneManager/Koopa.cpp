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
#include "FireVenusTrap.h"
#include "SpecialBrick.h"
#include "Button.h"
#include "Effect.h"

CKoopa::CKoopa(float x, float y, int type, int level) :CGameObject(x, y)
{
	ax = 0;
	ay = KOOPA_GRAVITY;
	this->type = type;
	vx = 0;
	this->level = level;
	SetState(GOOMBA_STATE_WALKING);
}


int CKoopa::GetAniIdRed()
{
	int aniId = -1;
	if (isFlipped)
	{
		if (state == KOOPA_STATE_DEFEND||state==KOOPA_STATE_DIE)
			aniId = ID_ANI_RED_KOOPA_FLIP_DEFEND;
		else if (state == KOOPA_STATE_BACK)
			aniId = ID_ANI_RED_KOOPA_FLIP_BACK;
		else
			aniId = ID_ANI_RED_KOOPA_FLIP_KICKED;
	}
	else
	{
		
			if (vx == KOOPA_WALKING_SPEED)
				aniId = ID_ANI_RED_KOOPA_WALK_RIGHT;
			else if (vx == -KOOPA_WALKING_SPEED)
				aniId = ID_ANI_RED_KOOPA_WALK_LEFT;
			else if (abs(vx) == KOOPA_KICKED_SPEED)
				aniId = ID_ANI_RED_KOOPA_KICKED;
			else
			{
				if (state == KOOPA_STATE_DEFEND || state == KOOPA_STATE_DIE)
					aniId = ID_ANI_RED_KOOPA_DEFEND;
				else
					aniId = ID_ANI_RED_KOOPA_BACK;
			}
		
	}
	return aniId;
}

int CKoopa::GetAniIdGreen()
{
	int aniId = -1;
	if (isFlipped)
	{
		if (state == KOOPA_STATE_DEFEND || state == KOOPA_STATE_DIE)
			aniId = ID_ANI_GREEN_KOOPA_FLIP_DEFEND;
		else if (state == KOOPA_STATE_BACK)
			aniId = ID_ANI_GREEN_KOOPA_FLIP_BACK;
		else
			aniId = ID_ANI_GREEN_KOOPA_FLIP_KICKED;
	}
	else
	{
		if (level == KOOPA_LEVEL_NORMAL)
		{
			if (vx == KOOPA_WALKING_SPEED)
				aniId = ID_ANI_GREEN_KOOPA_WALK_RIGHT;
			else if (vx == -KOOPA_WALKING_SPEED)
				aniId = ID_ANI_GREEN_KOOPA_WALK_LEFT;
			else if (abs(vx) == KOOPA_KICKED_SPEED)
				aniId = ID_ANI_GREEN_KOOPA_KICKED;
			else
			{
				if (state == KOOPA_STATE_DEFEND || state == KOOPA_STATE_DIE)
					aniId = ID_ANI_GREEN_KOOPA_DEFEND;
				else
					aniId = ID_ANI_GREEN_KOOPA_BACK;
			}
		}
		else
		{
			if (vx > 0)
				aniId = ID_ANI_GREEN_KOOPA_WINGS_WALK_RIGHT;
			else
				aniId = ID_ANI_GREEN_KOOPA_WINGS_WALK_LEFT;
		}
	}
	return aniId;

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
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float xM, yM, vxM, vyM;
	mario->GetPosition(xM, yM);
	mario->GetSpeed(vxM, vyM);
	if (!isInCam())	return;
	if (mario->GetIsTransforming()) return;
	if (level == KOOPA_LEVEL_WINGS)
	{
		if (isOnPlatForm) vy = KOOPA_JUMPING_SPEED;
	}
	if (vy != 0) isOnPlatForm = false;
	vx += ax * dt;
	vy += ay * dt;
	if (state == KOOPA_STATE_DIE)
	{
		if (GetTickCount64() - die_start > KOOPA_DIE_TIME)
		{
			Delete();
			
		}
		
	}
	if (state == KOOPA_STATE_UP)
	{
		if (isOnPlatForm) SetState(KOOPA_STATE_DEFEND);
		else this->vx = nx * MARIO_TAIL_ATTACK_SPEED_X;
	}
	if (state == KOOPA_STATE_DEFEND)
	{
		if (mario->GetIsHolding() && isHeld) {
			x = xM + mario->GetNX() * (KOOPA_BBOX_WIDTH - 5);
			y = yM - 3;
			nx = mario->GetNX();
			vx = vxM;
			vy = vyM;
		}
		else {
			if (this->isHeld) {
				mario->Kick();
				ay = KOOPA_GRAVITY;
				SetState(KOOPA_STATE_KICKED);
			}
		}
		
		if (GetTickCount64() - defend_start > KOOPA_DEFEND_TIMEOUT)
		{
			SetState(KOOPA_STATE_BACK);
		}
	}
	if (state == KOOPA_STATE_BACK)
	{
		if (GetTickCount64() - back_start > KOOPA_BACK_TIMEOUT)
		{
			SetState(KOOPA_STATE_WALKING);
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	int aniId = -1;
	if (type == KOOPA_TYPE_RED)
		aniId = GetAniIdRed();
	else
		aniId = GetAniIdGreen();
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
	if (state == KOOPA_STATE_DIE) return;
	if (GetState() == KOOPA_STATE_KICKED||GetState()==KOOPA_STATE_DEFEND)
	{
		if (dynamic_cast<CGoomba*>(e->obj)) {
			OnCollisionWithGoomba(e);
		}
		else if (dynamic_cast<CKoopa*>(e->obj))
			OnCollisionWithKoopa(e);
		else if (dynamic_cast<CFireVenusTrap*>(e->obj)) {
			OnCollisionWithFireVenusTrap(e);
		}
		else if (dynamic_cast<CSpecialBrick*>(e->obj)) {
			OnCollisionWithSpecialBrick(e);
		}
		else if (dynamic_cast<CQuestionBrick*>(e->obj)) {
			OnCollisionWithQuestionBrick(e);
		}
		
	}
	else
	{
		
		if (dynamic_cast<CBlockKoopa*>(e->obj)&&level!=KOOPA_LEVEL_WINGS) {
			OnCollisionWithBlockKoopa(e);
		}
	}
	if (dynamic_cast<CSpecialPlatform*>(e->obj))
		OnCollisionWithSpecialPlatform(e);
	if (!e->obj->IsBlocking()) return;
	if (e->ny < 0) { 
		isOnPlatForm = true;
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
				isOnPlatForm = true;
			}
		}
		else
		{
			if (yPlat - y <= (KOOPA_BBOX_HEIGHT_DEFEND))
			{
				SetPosition(x, yPlat - KOOPA_BBOX_HEIGHT_DEFEND);
				vy = 0;
				isOnPlatForm = true;
			}
		}
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_DEFEND && isHeld||state==KOOPA_STATE_KICKED)
	{
		CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		goomba->SetState(GOOMBA_STATE_JUMP_DIE);
		goomba->SetSpeed(nx * MARIO_TAIL_ATTACK_SPEED_X, MARIO_TAIL_ATTACK_SPEED_Y);
	}
	
}

void CKoopa::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_DEFEND && isHeld || state == KOOPA_STATE_KICKED)
	{
		CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
		CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
		koopa->SetIsFlipped(true);
		koopa->SetLevel(KOOPA_LEVEL_NORMAL);
		koopa->SetState(KOOPA_STATE_DIE);
	}
}

void CKoopa::OnCollisionWithFireVenusTrap(LPCOLLISIONEVENT e)
{
	CEffect* effect = new CEffect(x, y, EFFECT_SCORE_100);
	CFireVenusTrap* trap = dynamic_cast<CFireVenusTrap*>(e->obj);
	trap->SetState(FIREVENUSTRAP_STATE_DIE);
	if (state == KOOPA_STATE_DEFEND && isHeld)
	{
		SetState(KOOPA_STATE_DIE);
	}
}

void CKoopa::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e)
{
	CQuestionBrick* questionBrick = dynamic_cast<CQuestionBrick*>(e->obj);
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	if (state != KOOPA_STATE_KICKED) return;
	if (!questionBrick->GetIsEmpty())
	{
		questionBrick->SetState(QUESTION_BRICK_STATE_UP);

		if (questionBrick->GetType() == QUESTION_BRICK_TYPE_ITEM)
		{
			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				CMushRoom* mushroom = new CMushRoom(x+KOOPA_BBOX_WIDTH, y, MUSHROOM_TYPE_RED);
				mushroom->SetState(MUSHROOM_STATE_UP);
				scene->AddObject(mushroom);
			}
			else
			{
				CLeaf* leaf = new CLeaf(x + KOOPA_BBOX_WIDTH, y);
				leaf->SetState(LEAF_STATE_UP);
				scene->AddObject(leaf);
			}
		}
	}
}

void CKoopa::OnCollisionWithSpecialBrick(LPCOLLISIONEVENT e)
{
	float x, y;
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CSpecialBrick* specialBrick = dynamic_cast<CSpecialBrick*>(e->obj);
	specialBrick->GetPosition(x, y);
	if (state != KOOPA_STATE_KICKED) return;
	if (e->nx != 0)
	{
		if (!specialBrick->GetIsEmpty())
		{
			specialBrick->SetState(QUESTION_BRICK_STATE_UP);
			if (specialBrick->GetType() == SPECIAL_BRICK_TYPE_NORMAL)
			{
				specialBrick->Break();
				specialBrick->Delete();

			}
			if (specialBrick->GetType() == SPECIAL_BRICK_TYPE_MUSHROOM)
			{
				CMushRoom* mushroom = new CMushRoom(x, y, MUSHROOM_TYPE_GREEN);
				mushroom->SetState(MUSHROOM_STATE_UP);
				scene->AddObject(mushroom);
			}
			if (specialBrick->GetType() == SPECIAL_BRICK_TYPE_BUTTON)
			{
				CButton* button = new CButton(x, y - BUTTON_BBOX_HEIGHT);
				scene->AddObject(button);
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
		isFlipped = false;
		y -= (KOOPA_BBOX_HEIGHT + KOOPA_BBOX_HEIGHT_DEFEND) / 2;
		if (vx>0)
			vx = KOOPA_WALKING_SPEED;
		else
			vx = -KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_DEFEND:
		defend_start = GetTickCount64();
		y -= KOOPA_BBOX_HEIGHT / 4;
		vx = 0;
		break;
	case KOOPA_STATE_KICKED:
		if (nx >= 0)
			vx = KOOPA_KICKED_SPEED;
		else if (nx < 0)
			vx = -KOOPA_KICKED_SPEED;
		break;
	case KOOPA_STATE_BACK:
		back_start = GetTickCount64();
		break;
	case KOOPA_STATE_UP:
		isOnPlatForm = false;
		isFlipped = true;
		vy = MARIO_TAIL_ATTACK_SPEED_Y;
		break;
	case KOOPA_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		ay = 2 * KOOPA_GRAVITY;
		vy = MARIO_TAIL_ATTACK_SPEED_Y*1.5;
		level = 0;
		break;
	}
}
