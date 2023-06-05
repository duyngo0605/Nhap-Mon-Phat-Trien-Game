#pragma once
#include "Platform.h"
class CSpecialPlatform :public CPlatform //only block with down moving
{
public:
	int IsBlocking() { return 0; }
};

