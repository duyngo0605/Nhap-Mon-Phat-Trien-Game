#include"GameObject.h"

#define KOOPA_GRAVITY 0.001f
#define KOOPA_WALKING_SPEED 0.03f
#define KOOPA_JUMPING_SPEED -0.3f
#define KOOPA_KICKED_SPEED 0.2f


#define KOOPA_DEFEND_TIMEOUT 8000 
#define KOOPA_BACK_TIMEOUT 1000 

#define KOOPA_DIE_TIME 2000

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DEFEND 16


#define KOOPA_GREEN 1
#define KOOPA_RED 2
#define KOOPA_GREEN_WING 3

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DEFEND 200
#define KOOPA_STATE_KICKED 300
#define KOOPA_STATE_BACK 400
#define KOOPA_STATE_UP 500
#define KOOPA_STATE_DIE 900

#define KOOPA_TYPE_RED 1
#define KOOPA_TYPE_GREEN 2

#define ID_ANI_RED_KOOPA_WALK_LEFT 18100
#define ID_ANI_RED_KOOPA_WALK_RIGHT 18101
#define ID_ANI_RED_KOOPA_DEFEND 18200
#define ID_ANI_RED_KOOPA_KICKED 18201
#define ID_ANI_RED_KOOPA_BACK 18300

#define ID_ANI_RED_KOOPA_FLIP_DEFEND 18210
#define ID_ANI_RED_KOOPA_FLIP_KICKED 18211
#define ID_ANI_RED_KOOPA_FLIP_BACK 18310


#define ID_ANI_GREEN_KOOPA_WALK_LEFT 18600
#define ID_ANI_GREEN_KOOPA_WALK_RIGHT 18601
#define ID_ANI_GREEN_KOOPA_DEFEND 18700
#define ID_ANI_GREEN_KOOPA_KICKED 18701
#define ID_ANI_GREEN_KOOPA_BACK 18800

#define ID_ANI_GREEN_KOOPA_FLIP_DEFEND 18710
#define ID_ANI_GREEN_KOOPA_FLIP_KICKED 18711
#define ID_ANI_GREEN_KOOPA_FLIP_BACK 18810

#define ID_ANI_GREEN_KOOPA_WINGS_WALK_LEFT 18901
#define ID_ANI_GREEN_KOOPA_WINGS_WALK_RIGHT 18900

#define KOOPA_LEVEL_NORMAL 1
#define KOOPA_LEVEL_WINGS 2

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;
	int type;
	ULONGLONG defend_start;
	ULONGLONG die_start;
	ULONGLONG back_start;

	int level;

	int GetAniIdRed();
	int GetAniIdGreen();
	bool isHeld = false;
	bool isOnPlatForm;
	bool isFlipped;
public:
	CKoopa(float x, float y, int type, int level);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return (state!=KOOPA_STATE_DIE); };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithSpecialPlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithSpecialBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithBlockKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithFireVenusTrap(LPCOLLISIONEVENT e);

	void SetState(int state);
	void SetIsHeld(bool isHeld)
	{
		this->isHeld = isHeld;
	}
	void SetLevel(int level) { this->level = level; }
	int GetLevel() { return level; }
};