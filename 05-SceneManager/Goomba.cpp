#include "Goomba.h"
#include "SpecialPlatform.h"

CGoomba::CGoomba(float x, float y, int type):CGameObject(x, y)
{
	xStart = x;
	yStart = y;
	this->type = type;
	if (type == GOOMBA_TYPE_NORMAL) level = 1;
	if (type == GOOMBA_TYPE_PARA)	level = 2;
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	vx = 0;
	die_start = walk_start = prepare_fly_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		if (level == 1)
		{
			left = x - GOOMBA_BBOX_WIDTH / 2;
			top = y - GOOMBA_BBOX_HEIGHT / 2;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT;
		}
		else if (level==2)
		{
			left = x - PARAGOOMBA_BBOX_WIDTH / 2;
			top = y - PARAGOOMBA_BBOX_HEIGHT / 2;
			right = left + PARAGOOMBA_BBOX_WIDTH;
			bottom = top + PARAGOOMBA_BBOX_HEIGHT;
		}
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!isInCam()) return;
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::OnCollisionWithSpecialPlatform(LPCOLLISIONEVENT e)
{
	CSpecialPlatform* specialPlat = dynamic_cast<CSpecialPlatform*>(e->obj);
	float xPlat, yPlat;
	specialPlat->GetPosition(xPlat, yPlat);
	if (e->ny < 0) {
		if (state==GOOMBA_STATE_DIE)
		{
			if (yPlat - y <= (GOOMBA_BBOX_HEIGHT_DIE + SPECIAL_PLATFORM_BBOX_HEIGHT) / 2)
			{
				SetPosition(x, yPlat - (GOOMBA_BBOX_HEIGHT_DIE + SPECIAL_PLATFORM_BBOX_HEIGHT) / 2);
				vy = 0;
			}
		}
		else
		{
			if (level == 1)
			{
				if (yPlat - y <= (GOOMBA_BBOX_HEIGHT + SPECIAL_PLATFORM_BBOX_HEIGHT) / 2)
				{
					SetPosition(x, yPlat - (GOOMBA_BBOX_HEIGHT + SPECIAL_PLATFORM_BBOX_HEIGHT) / 2);
					vy = 0;
				}
			}
			else if (level == 2)
			{
				if (yPlat - y <= (PARAGOOMBA_BBOX_HEIGHT + SPECIAL_PLATFORM_BBOX_HEIGHT) / 2)
				{
					SetPosition(x, yPlat - (PARAGOOMBA_BBOX_HEIGHT + SPECIAL_PLATFORM_BBOX_HEIGHT) / 2);
					vy = 0;
				}
			}
		}
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (!isInCam()) return;
	vy += ay * dt;
	vx += ax * dt;
	
	if (level == 0) SetState(GOOMBA_STATE_DIE);
	if (GetState()==GOOMBA_STATE_DIE&&(GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		Delete();
		return;
	}
	if (level == 2)
	{
		if (state == GOOMBA_STATE_WALKING)
		{
			if (GetTickCount64() - walk_start > GOOMBA_WALK_TIMEOUT)
				SetState(GOOMBA_STATE_PREPARE_FLY);
		}
		if (state == GOOMBA_STATE_PREPARE_FLY)
		{
			if (GetTickCount64() - prepare_fly_start > GOOMBA_PREPARE_FLY_TIMEOUT)
				SetState(GOOMBA_STATE_FLYING);
		}
		if (state == GOOMBA_STATE_FLYING)
		{
			if (GetTickCount64()-fly_start>GOOMBA_FLY_TIMEOUT) 
				SetState(GOOMBA_STATE_WALKING);
		}
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	CGameObject::Render();
	int aniId;
	if (type == GOOMBA_TYPE_NORMAL)
	{
		aniId = ID_ANI_GOOMBA_WALKING;
		if (state == GOOMBA_STATE_DIE)
		{
			aniId = ID_ANI_GOOMBA_DIE;
		}
	}
	else if (type == GOOMBA_TYPE_PARA)
	{
		if (state == GOOMBA_STATE_DIE)
		{
			aniId = ID_ANI_PARAGOOMBA_DIE;
		}
		if (level == 1)
		{
			aniId = ID_ANI_PARAGOOMBA_WALKING;
		}
		else if (level==2)
		{
			if (state == GOOMBA_STATE_WALKING)
				aniId = ID_ANI_PARAGOOMBA_WINGS_WALKING;
			else if (state == GOOMBA_STATE_PREPARE_FLY)
				aniId = ID_ANI_PARAGOOMBA_PREPARE_FLY;
			else if (state == GOOMBA_STATE_FLYING)
			{
				if (nx<0) aniId = ID_ANI_PARAGOOMBA_FLY_LEFT;
				else  aniId = ID_ANI_PARAGOOMBA_FLY_RIGHT;
			}
				
		}
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (this->state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
			level=-1;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			walk_start = GetTickCount64();
			ay = GOOMBA_GRAVITY;
			ax = 0;
			if( vx>0) vx = GOOMBA_WALKING_SPEED;
			else
				vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_FLYING:
			fly_start = GetTickCount64();
			vy = GOOMBA_FLYING_SPEED;
			this->vx = this->vx * 2;
			ay = ay / 2;
			
			
			break;
		case GOOMBA_STATE_PREPARE_FLY:
			prepare_fly_start = GetTickCount64();
			break;
	}
}
