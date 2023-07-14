#pragma once

#define ID_HUD	999999
#define ID_NUMBER_0	0
#define ID_NUMBER_1	1
#define ID_NUMBER_2	2
#define ID_NUMBER_3	3
#define ID_NUMBER_4	4
#define ID_NUMBER_5	5
#define ID_NUMBER_6	6
#define ID_NUMBER_7	7
#define ID_NUMBER_8	8
#define ID_NUMBER_9	9

#define ID_RUNLEVEL	10
#define ID_P		11	//can fly

#define HUD_WIDTH	270.0f
#define HUD_HEIGHT 32.0f



class CHud
{
	static CHud* __hudInstance;
	float x, y;
public:
	CHud(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	static CHud* GetInstance();

	void Update();
	void Render();
	void RenderCoin();
	void RenderHP();
	void RenderScore();
	void RenderTimer();
};

