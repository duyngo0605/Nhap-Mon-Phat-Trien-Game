#include"GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.04f
#define KOOPA_THROWN_SPEED 0.2f


#define KOOPA_DEFEND_TIMEOUT 4000 
#define KOOPA_BACK_TIMEOUT 1000 

#define KOOPA_DIE_TIME 1000

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DEFEND 16


#define KOOPA_GREEN 1
#define KOOPA_RED 2
#define KOOPA_GREEN_WING 3

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DEFEND 200
#define KOOPA_STATE_THROWN 300

#define KOOPA_TYPE_RED 1
#define KOOPA_TYPE_GREEN 2

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	int type;
	ULONGLONG defend_start;
	ULONGLONG die_start;
	ULONGLONG back_start;

public:
	CKoopa(float x, float y, int type);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);

	void SetState(int state);


};