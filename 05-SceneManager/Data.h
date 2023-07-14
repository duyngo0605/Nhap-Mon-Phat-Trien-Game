#pragma once
#include "Node.h"
#include "Card.h"

#define LIMIT_TIME 300

///store the data to link 1-1 and worldmap HUD
class CData
{
	static CData* __dataInstance;
public:

	static int coin;
	static int HP;
	static int score;
	static CCard cardGot[3];
	static int marioLevel;
	static int timer;
	static float x;
	static float y;
	static int marioRunLevel;
	CData(){}
	~CData(){}
	static CData* GetInstance()
	{
		if (!__dataInstance)
			__dataInstance = new CData();
		return __dataInstance;
	}
	void SetCard(int type);
};

