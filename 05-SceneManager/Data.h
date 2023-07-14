#pragma once
#include "Node.h"
#include "Card.h"


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
	static float marioX;
	static float marioY;
	CData(){}
	~CData(){}
	static CData* GetInstance()
	{
		if (!__dataInstance)
			__dataInstance = new CData();
		return __dataInstance;
	}

};

